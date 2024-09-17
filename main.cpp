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
    int pid;
    int arrival_time;
    int burst_time;
    int rem_time;
    int priority;
    int wait_time;
    bool finished;

public:
    Task(int pid, int at, int bt, int pr)
        : pid(pid), arrival_time(at), burst_time(bt), rem_time(bt), priority(pr), wait_time(0), finished(false) {}

    int getRemTime() const { return rem_time; }
    int getPriority() const { return priority; }
    int getPid() const { return pid; }
    int getArrivalTime() const { return arrival_time; }
    int getBurstTime() const { return burst_time; }
    void decreaseRemTime(int time) { rem_time -= time; }
    void setRemTime(int time) { rem_time = time; }
    void increasePriority(int change)
    {
        if (priority > 1)
            priority -= change;
    }
    int getWaitTime() const { return wait_time; }
    bool getFinished() const { return finished; }
    void setFinished(bool cond) { finished = cond; }
    void setWaitingTime(int time) { wait_time = time; }
};

// TaskManager class
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

// Compare class for priority scheduling
class Compare
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
class ShortestJobFirst  {
private:
    TaskManager & tm;

public:
    ShortestJobFirst(TaskManager &tm) : tm(tm) {}

    void Schedule()  {
        vector<Task*> tasks  = tm.getTasks();
        int n = tasks.size();

        int curr_time = 0;
        int completed =0;

        while(completed < n){
            Task* shortest = nullptr;

            for(auto &task : tasks){
                if(!task->getFinished() && task->getArrivalTime() <= curr_time){
                    if(shortest == nullptr || shortest->getBurstTime() > task->getBurstTime()){
                        shortest = task;
                    }
                }
            }

            if(shortest == nullptr){
                curr_time++;
                continue;
            }

            else{
                curr_time += shortest->getBurstTime();
                cout<<"Process id "<<shortest->getPid() << " executed for "<< shortest->getBurstTime() << " Current Time "<< curr_time<<endl;
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

    void ageTasks(int curr_time, Task *m)
    {
        vector<Task *> tasks = taskmanager.getTasks();

        for (auto &task : tasks)
        {
            if (task->getRemTime() > 0 && !task->getFinished() && task->getArrivalTime() <= curr_time && task->getPid() != m->getPid())
            {
                task->setWaitingTime(task->getWaitTime() + 1);
                if (task->getWaitTime() % 2 == 0)
                {
                    task->increasePriority(1);
                    cout << "Task " << task->getPid() << " Priority increased to " << task->getPriority() << endl;
                }
            }
        }
    }
};
class multiLevelQueue
{
private:
    TaskManager &tm;
    queue<Task *> highest;
    queue<Task *> middle;
    queue<Task *> lower;
    int timequantum;

public:
    multiLevelQueue(TaskManager &tm, int timeq) : tm(tm), timequantum(timeq) {}

    void Schedule()
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

            // Process highest priority queue
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

            // Process middle priority queue
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
                        // If a higher-priority task arrives, preempt the current task
                        middle.push(t); // Push it back to lower queue
                        continue;
                    }
                }

                if (t->getRemTime() > 0)
                {
                    middle.push(t); // Re-add the task if it's not finished
                }
                else
                {
                    cout << "Process id " << t->getPid() << " Finished at " << curr_time << endl;
                }
            }

            // Process lower priority queue
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
                        // If a higher-priority task arrives, preempt the current task
                        // lower.push(t); // Push it back to lower queue
                        if (t->getRemTime() > 0)
                        {
                            lower.push(t);
                        }
                        continue;
                    }
                }

                if (t->getRemTime() > 0)
                {
                    lower.push(t); // Re-add the task if it's not finished
                }
                else
                {
                    cout << "Process id " << t->getPid() << " Finished at " << curr_time << endl;
                }
            }

            // Move to next time if all queues are empty and tasks are left
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
        new Task(5, 0, 4, 9), // Task 1: Arrival 0, Burst 10, Priority 9 (Lowest priority)
        new Task(4, 2, 2, 5), // Task 3: Arrival 2, Burst 20, Priority 5 (Middle priority)
        new Task(1, 3, 5, 4), // Task 4: Arrival 3, Burst 25, Priority 4 (Middle priority)
        new Task(3, 4, 3, 2), // Task 5: Arrival 4, Burst 30, Priority 2 (High priority)
        new Task(2, 5, 6, 1), // Task 6: Arrival 5, Burst 35, Priority 1 (Highest priority)

    };

    TaskManager tm(tasks);

    // Priority Scheduling
    // cout << "Priority Scheduling:\n";
    // PriorityScheduling ps(tm);
    // ps.Schedule();
    // ps.CalculateMetrics();

    // // Round Robin Scheduling
    // cout << "Round Robin Scheduling:\n";
    // RoundRobin rf(tm, 2);
    // rf.Schedule();
    // rf.CalculateMetrics();

    // Shortest Job First Scheduling
    cout << "Shortest Job First Scheduling:\n";
    ShortestJobFirst sjf(tm);
    sjf.Schedule();
    

    // multiLevelQueue mlq(tm, 3);
    // mlq.Schedule();

    return 0;
}


// #include <iostream>
// #include "Headers/TaskManager.hpp"
// #include "Headers/RoundRobin.hpp"
// #include "Headers/ShortestJobFirst.hpp"
// #include "Headers/PriorityScheduling.hpp"
// #include "Headers/MultiLevelQueue.hpp"


// #include<vector>

// using namespace std;

// int main() {
//     vector<Task *> tasks = {
//         new Task(5, 0, 4, 9), // Task 1: Arrival 0, Burst 10, Priority 9 (Lowest priority)
//         new Task(4, 2, 2, 5), // Task 3: Arrival 2, Burst 20, Priority 5 (Middle priority)
//         new Task(1, 3, 5, 4), // Task 4: Arrival 3, Burst 25, Priority 4 (Middle priority)
//         new Task(3, 4, 3, 2), // Task 5: Arrival 4, Burst 30, Priority 2 (High priority)
//         new Task(2, 5, 6, 1), // Task 6: Arrival 5, Burst 35, Priority 1 (Highest priority)

//     };
//     TaskManager tm(tasks);
//     // Select scheduler
//     RoundRobin rr(tm, 2);
//     rr.Schedule();
//     rr.CalculateMetrics();
    
//     // You can similarly instantiate ShortestJobFirst, PriorityScheduling, etc.

//     return 0;
// }
