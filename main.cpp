#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <chrono>
#include <stdexcept>

using namespace std;

// Base scheduler class
class Schedular
{
public:
    virtual void Schedule() = 0;
    virtual void CalculateMetrics() = 0;
    virtual ~Schedular() {}
};

// Task class
class Task
{
private:
    const int pid;
    int arrival_time;
    int burst_time;
    int rem_time;
    int priority;
    int wait_time;
    bool finished;

public:
    Task(int pid, int at, int bt, int pr)
        : pid(pid), arrival_time(at), burst_time(bt), rem_time(bt), priority(pr), wait_time(0), finished(false) {}

    int getRemTime() const
    {
        return rem_time;
    }
    int getPriority() const
    {
        return priority;
    }
    int getPid() const
    {
        return pid;
    }
    int getArrivalTime() const
    {
        return arrival_time;
    }
    int getBurstTime() const { return burst_time; }
    void decreaseRemTime(int time) { rem_time -= time; }
    void setRemTime(int time) { rem_time = time; }
    void increasePriority(int change)
    {
        if (priority > 1)
            priority -= change;
    }
    int getWaitTime() const
    {
        return wait_time;
    }
    bool getFinished() const
    {
        return finished;
    }
    void setFinished(bool cond) { finished = cond; }
    void setWaitingTime(int time) { wait_time = time; }
};

class TaskManager
{
private:
    vector<Task *> tasks;

public:
    TaskManager(vector<Task *> taskslist) : tasks(taskslist) {}

    ~TaskManager()
    {
        for (auto task : tasks)
        {
            delete task;
        }
        tasks.clear();
    }

    void addTask(Task *task)
    {
        tasks.push_back(task);
    }

    vector<Task *> &getTasks() { return tasks; }

    Task *getTaskById(int id)
    {
        for (auto task : tasks)
        {
            if (task->getPid() == id)
            {
                return task;
            }
        }
        throw runtime_error("Task not found");
    }
};

class Compare // for priority scheduling
{
public:
    bool operator()(Task *a, Task *b)
    {
        return a->getPriority() > b->getPriority();
    }
};

// RoundRobin class
class RoundRobin : public Schedular
{
private:
    TaskManager tm;
    int timequantum;
    vector<double> arrival;
    vector<double> finish;

public:
    RoundRobin(TaskManager tm, int tq) : tm(tm), timequantum(tq)
    {
        int n = tm.getTasks().size();
        arrival.resize(n, 0.0);
        finish.resize(n, 0.0);
    }

    void Schedule() override
    {
        vector<Task *> tasks = tm.getTasks();
        sort(tasks.begin(), tasks.end(), [](Task *a, Task *b)
             { return a->getArrivalTime() < b->getArrivalTime(); });
        queue<Task *> q;

        int curr_time = 0;
        int i = 0;

        while (!q.empty() || i < tasks.size())
        {
            while (i < tasks.size() && tasks[i]->getArrivalTime() <= curr_time)
            {
                arrival[tasks[i]->getPid() - 1] = tasks[i]->getArrivalTime();
                q.push(tasks[i]);
                i++;
            }

            if (!q.empty())
            {
                Task *t = q.front();
                q.pop();

                int time_executed = min(t->getRemTime(), this->timequantum);
                curr_time += time_executed;
                t->decreaseRemTime(time_executed);
                cout << "Process id " << t->getPid() << " Executed for " << time_executed
                     << " ms Remaining time " << t->getRemTime() << ". Current Time " << curr_time << endl;
                if (t->getRemTime() > 0)
                {
                    q.push(t);
                }
                else
                {
                    cout << "Process id " << t->getPid() << " Finished at " << curr_time << endl;
                    finish[t->getPid() - 1] = curr_time;
                }
            }
            else
            {
                if (i < tasks.size())
                {
                    curr_time = tasks[i]->getArrivalTime();
                }
            }
        }
    }

    void CalculateMetrics() override
    {
        int n = tm.getTasks().size();
        double sum = 0;
        for (int i = 0; i < n; i++)
        {
            sum += finish[i] - arrival[i];
        }

        cout << "Avg turnaround time: " << sum / double(n) << endl
             << endl;
    }
};

// ShortestJobFirst class
class ShortestJobFirst
{
private:
    TaskManager &tm;

public:
    ShortestJobFirst(TaskManager &tm) : tm(tm) {}

    void Schedule()
    {
        vector<Task *> tasks = tm.getTasks();
        int n = tasks.size();

        int curr_time = 0;
        int completed = 0;

        while (completed < n)
        {
            Task *shortest = nullptr;

            for (auto &task : tasks)
            {
                if (!task->getFinished() && task->getArrivalTime() <= curr_time)
                {
                    if (shortest == nullptr || shortest->getBurstTime() > task->getBurstTime())
                    {
                        shortest = task;
                    }
                }
            }

            if (shortest == nullptr)
            {
                curr_time++;
                continue;
            }

            else
            {
                curr_time += shortest->getBurstTime();
                cout << "Process id " << shortest->getPid() << " executed for " << shortest->getBurstTime() << " Current Time " << curr_time << endl;
                shortest->setFinished(true);
                completed++;
            }
        }
    }
};

// PriorityScheduling class
class PriorityScheduling : public Schedular
{
private:
    TaskManager &taskmanager;
    vector<double> arrive;
    vector<double> finish;

public:
    PriorityScheduling(TaskManager &tm) : taskmanager(tm)
    {
        int n = taskmanager.getTasks().size();
        arrive.resize(n, 0.0);
        finish.resize(n, 0.0);
    }

    void Schedule() override
    {
        vector<Task *> &tasks = taskmanager.getTasks();
        priority_queue<Task *, vector<Task *>, Compare> pq;
        sort(tasks.begin(), tasks.end(), [](Task *a, Task *b)
             { return a->getArrivalTime() < b->getArrivalTime(); });

        int idx = 0;
        int curr_time = 0;
        int n = tasks.size();

        while (!pq.empty() || idx < n)
        {
            while (idx < n && curr_time >= tasks[idx]->getArrivalTime())
            {
                arrive[tasks[idx]->getPid() - 1] = tasks[idx]->getArrivalTime();
                pq.push(tasks[idx]);
                idx++;
            }
            ageTasks(curr_time, !pq.empty() ? pq.top() : nullptr, pq);
            if (!pq.empty())
            {
                Task *m = pq.top();
                pq.pop();

                int time_Start = curr_time;
                while (m->getRemTime() > 0)
                {

                    if (idx < n && tasks[idx]->getArrivalTime() <= curr_time && tasks[idx]->getPriority() < m->getPriority())
                    {
                        pq.push(m);
                        break; // Higher priority process arrived
                    }
                    curr_time++;
                    m->decreaseRemTime(1);
                }
                cout << "Process id " << m->getPid()
                     << " Executed For " << curr_time - time_Start << " ms. Current Time " << curr_time << endl;
                if (m->getRemTime() > 0)
                {
                    pq.push(m);
                }
                else
                {
                    cout << "Process id " << m->getPid() << " Finished at " << curr_time << endl;
                    m->setFinished(true);
                    finish[m->getPid() - 1] = curr_time;
                }
            }
            else
            {
                if (idx < n)
                    curr_time = tasks[idx]->getArrivalTime();
            }
        }
    }

    void CalculateMetrics() override
    {
        int n = taskmanager.getTasks().size();

        double sum = 0;
        for (int i = 0; i < n; i++)
        {
            sum += finish[i] - arrive[i];
        }
        cout << "Average TAT " << sum / (double)n << endl;
    }

    void ageTasks(int curr_time, Task *m, priority_queue<Task *, vector<Task *>, Compare> &pq)
    {
        vector<Task *> tasks = taskmanager.getTasks();
        bool changed = false;

        if (m != nullptr)
        {
            for (auto &task : tasks)
            {
                if (task->getRemTime() > 0 && !task->getFinished() && task->getArrivalTime() <= curr_time && task->getPid() != m->getPid())
                {
                    task->setWaitingTime(task->getWaitTime() + 1);
                    if (task->getWaitTime() % 4 == 0)
                    {
                        if (task->getPriority() > 1)
                        {
                            task->increasePriority(1);
                            cout << "Task " << task->getPid() << " Priority increased to " << task->getPriority() << endl;
                            changed = true;
                        }
                    }
                }
            }

            if (changed)
            {
                priority_queue<Task *, vector<Task *>, Compare> pqn;

                while (!pq.empty())
                {
                    auto m = pq.top();
                    pq.pop();
                    pqn.push(m);
                }

                pq = pqn;
            }
        }
    }
};
class multiLevelQueue : public Schedular
{
private:
    TaskManager &tm;
    queue<Task *> highest;
    queue<Task *> middle;
    queue<Task *> lower;
    int timequantum;

public:
    multiLevelQueue(TaskManager &tm, int timeq) : tm(tm), timequantum(timeq) {}

    void Schedule() override
    {
        int curr_time = 0;
        int idx = 0;

        vector<Task *> tasks = tm.getTasks();
        int n = tasks.size();
        sort(tasks.begin(), tasks.end(), [](Task *a, Task *b)
             { return a->getArrivalTime() < b->getArrivalTime(); });

        while (idx < n || !highest.empty() || !middle.empty() || !lower.empty())
        {
            while (idx < n && curr_time >= tasks[idx]->getArrivalTime())
            {
                int priority = tasks[idx]->getPriority();

                if (priority <= 3 && priority >= 1)
                {
                    highest.push(tasks[idx]);
                }
                else if (priority >= 4 && priority <= 6)
                {
                    middle.push(tasks[idx]);
                }
                else if (priority >= 6 && priority <= 9)
                {
                    lower.push(tasks[idx]);
                }
                idx++;
            }

            if (!highest.empty())
            {
                Task *t = highest.front();
                highest.pop();

                int time_executed = min(t->getRemTime(), this->timequantum);
                curr_time += time_executed;
                t->decreaseRemTime(time_executed);
                cout << "Process id " << t->getPid() << " Executed for " << time_executed
                     << " ms Remaining time " << t->getRemTime() << ". Current Time " << curr_time << endl;
                if (t->getRemTime() > 0)
                {
                    highest.push(t);
                }
                else
                {
                    cout << "Process id " << t->getPid() << " Finished at " << curr_time << endl;
                }
            }

            else if (!middle.empty())
            {
                Task *t = middle.front();
                middle.pop();

                int time_executed = min(t->getRemTime(), this->timequantum);
                curr_time += time_executed;
                t->decreaseRemTime(time_executed);
                cout << "Process id " << t->getPid() << " Executed for " << time_executed
                     << " ms Remaining time " << t->getRemTime() << ". Current Time " << curr_time << endl;

                if ((idx < n && curr_time >= tasks[idx]->getArrivalTime()))
                {
                    int priority = tasks[idx]->getPriority();
                    if (priority <= 6 && priority >= 4)
                    {
                        if (t->getRemTime() > 0)
                        {
                            middle.push(t); // Re-add the task if it's not finished
                        }
                        continue;
                    }
                }

                if (t->getRemTime() > 0)
                {
                    middle.push(t);
                }
                else
                {
                    cout << "Process id " << t->getPid() << " Finished at " << curr_time << endl;
                }
            }

            else if (!lower.empty())
            {
                Task *t = lower.front();
                lower.pop();

                int time_executed = min(t->getRemTime(), this->timequantum);
                curr_time += time_executed;
                t->decreaseRemTime(time_executed);
                cout << "Process id " << t->getPid() << " Executed for " << time_executed
                     << " ms Remaining time " << t->getRemTime() << ". Current Time " << curr_time << endl;

                if ((idx < n && curr_time >= tasks[idx]->getArrivalTime()))
                {
                    int priority = tasks[idx]->getPriority();

                    if (priority <= 6 && priority >= 4)
                    {
                        if (t->getRemTime() > 0)
                        {
                            lower.push(t);
                        }
                        continue;
                    }
                }

                if (t->getRemTime() > 0)
                {
                    lower.push(t);
                }
                else
                {
                    cout << "Process id " << t->getPid() << " Finished at " << curr_time << endl;
                }
            }

            if (idx < tasks.size() && highest.empty() && middle.empty() && lower.empty())
            {
                curr_time = tasks[idx]->getArrivalTime();
            }
        }
    }
};

int main()
{
    vector<Task *> tasks = {
        new Task(1, 0, 12, 9), // Task 1: Arrival 0, Burst 12, Priority 9 (Lowest priority)
        new Task(2, 1, 10, 8), // Task 2: Arrival 1, Burst 10, Priority 8 (Low priority)
        new Task(3, 2, 8, 7),  // Task 3: Arrival 2, Burst 8, Priority 7 (Low priority)
        new Task(4, 3, 6, 2),  // Task 4: Arrival 3, Burst 6, Priority 2 (High priority)
        new Task(5, 4, 4, 1),  // Task 5: Arrival 4, Burst 4, Priority 1 (Highest priority)
        new Task(6, 5, 14, 5), // Task 6: Arrival 5, Burst 14, Priority 5 (Middle priority)
        new Task(7, 6, 9, 6),  // Task 7: Arrival 6, Burst 9, Priority 6 (Low priority)
        new Task(8, 7, 11, 3), // Task 8: Arrival 7, Burst 11, Priority 3 (High priority)
        new Task(9, 8, 7, 4),  // Task 9: Arrival 8, Burst 7, Priority 4 (Middle priority)
        new Task(10, 9, 5, 2)  // Task 10: Arrival 9, Burst 5, Priority 2 (High priority)
    };
    TaskManager tm(tasks);

    // multiLevelQueue mlq(tm, 3);
    // mlq.Schedule();

    PriorityScheduling prs(tm);
    prs.Schedule();
    prs.CalculateMetrics();

    return 0;
}
