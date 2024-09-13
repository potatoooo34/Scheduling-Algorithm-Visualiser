#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <chrono>

using namespace std;
// create a class with pid , burst time and arrival time , priority
class Schedular
{
public:
    virtual void Schedule() = 0;
    virtual void CalculateMetrics() = 0;
    virtual ~Schedular() {}
};
class Task
{
private:
    int pid;
    int arrival_time;
    int burst_time;
    int rem_time;
    int priority;

public:
    Task(int pid, int at, int bt, int pr)
    {
        this->pid = pid;
        this->arrival_time = at;
        this->burst_time = bt;
        this->rem_time = bt;
        this->priority = pr;
    }
    int getRemTime()
    {
        return this->rem_time;
    }
    int getPriority()
    {
        return this->priority;
    }
    int getPid()
    {
        return this->pid;
    }
    int getArrivalTime()
    {
        return this->arrival_time;
    }
    int getBurstTime()
    {
        return this->burst_time;
    }
    void decreaseRemTime(int time)
    {
        this->rem_time -= time;
    }
    void setRemTime(int time)
    {
        this->rem_time = time;
    }
};
class TaskManager
{
private:
    unordered_map<int , Task*> tasks;

public:
    TaskManager(vector<Task *> taskslist)
    {
        for(auto & task : taskslist){
            tasks[task->getPid()] = task;
        }
    }
    ~TaskManager() 
    {
        for (auto &pair : tasks)
        {
            delete pair.second; 
        }
        tasks.clear(); 
    }
    void addTasks(Task *task)
    {
        tasks[task->getPid()] = task;
    }
    vector<Task *> getTasks()
    {
        vector<Task*> task;
        for(auto &t : tasks){
            task.push_back(t.second);
        }
        return task;
    }
    Task* getTaskById(int id){
        if(tasks.find(id) != tasks.end()){
            return tasks[id];
        }

        return nullptr;
    }
};
class RoundRobin : public Schedular
{
private:
    TaskManager &tm;
    int timequantum;
    vector<double> arrival;
    vector<double> finish;

public:
    RoundRobin(TaskManager &tm, int tq) : tm(tm)
    {

        this->timequantum = tq;
        int n = tm.getTasks().size();
        arrival.resize(n, 0.0);
        finish.resize(n, 0.0);
    }

    void Schedule() override
    {

        int n = tm.getTasks().size();
        vector<Task *> tasks = tm.getTasks();
        sort(tasks.begin(), tasks.end(), [](Task *a, Task *b)
             { return a->getArrivalTime() < b->getArrivalTime(); });
        queue<Task *> q;

        int curr_time = 0;
        int i = 0;

        while (!q.empty() || i < n)
        {
            while (i < n && tasks[i]->getArrivalTime() <= curr_time)
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
                    t->setRemTime(t->getBurstTime());
                    finish[t->getPid() - 1] = curr_time;
                }
            }
            else
            {
                if (i < n)
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

class ShortestJobFirst : public Schedular
{
private:
    TaskManager &tm;
    vector<double> arrive;
    vector<double> finish;

public:
    ShortestJobFirst(TaskManager &tm) : tm(tm)
    {

        int n = tm.getTasks().size();
        arrive.resize(n, 0.0);
        finish.resize(n, 0.0);
    }

    void Schedule() override
    {
        vector<Task *> tasks = tm.getTasks();
        sort(tasks.begin(), tasks.end(), [](Task *a, Task *b)
             { return a->getBurstTime() < b->getBurstTime(); });
        int curr_time = 0;
        for (auto &it : tasks)
        {
            arrive[it->getPid() - 1] = it->getArrivalTime();
            curr_time = max(curr_time, it->getArrivalTime());
            cout << "Process id " << it->getPid()
                 << " Executed For " << it->getBurstTime() << " ms. Current Time " << curr_time << endl;
            curr_time += it->getBurstTime();
            finish[it->getPid() - 1] = it->getArrivalTime() + it->getBurstTime();
        }
    }

    void CalculateMetrics() override
    {
        double sum = 0;
        int n = tm.getTasks().size();

        for (int i = 0; i < n; i++)
        {
            sum += finish[i] - arrive[i];
        }

        cout << "Avg TAT " << sum / (double)n << endl
             << endl;
    }
};
class Compare
{
public:
    bool operator()(Task *a, Task *b)
    {
        return a->getPriority() > b->getPriority();
    }
};
// lower number higher priority
class PriorityScheduling : public Schedular
{
    TaskManager &taskmanager;
    vector<double> arrive;
    vector<double> finish;

public:
    PriorityScheduling(TaskManager &tm) :taskmanager(tm)
    {
        
        int n = taskmanager.getTasks().size();
        arrive.resize(n, 0.0);
        finish.resize(n, 0.0);
    }

    void Schedule() override
    {
        vector<Task*> tasks = taskmanager.getTasks();
        priority_queue<Task *, vector<Task *>, Compare> pq;
        sort(tasks.begin(), tasks.end(), [](Task *a, Task *b)
             { return a->getArrivalTime() < b->getArrivalTime(); });

        int idx = 0;
        int curr_time = 0;
        int n = tasks.size();
        int time_executed = 0;
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
                auto m = pq.top();
                pq.pop();
                int time_Start = curr_time;
                while (m->getRemTime() > 0)
                {
                    if (idx < n && tasks[idx]->getArrivalTime() <= curr_time && tasks[idx]->getPriority() < m->getPriority())
                    {
                        break; // highr priority process arrived;
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
                    finish[m->getPid() - 1] = curr_time;
                    m->setRemTime(m->getBurstTime());
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
        cout << "Average TAT " << sum / (double)n << endl
             << endl;
    }
};

class MultiLevelQueue{

};
int main()
{
    vector<Task *> tasks = {
        new Task(1, 4, 5, 3),  
        new Task(2, 2, 3, 1),  
        new Task(3, 0, 8, 2),  
        new Task(4, 10, 2, 4), 
        new Task(5, 6, 4, 2),  
        new Task(6, 11, 6, 5), 
        new Task(7, 12, 7, 1), 
        new Task(8, 10, 3, 3), 
        new Task(9, 13, 5, 4), 
        new Task(10, 18, 4, 2)
    };

    TaskManager tm(tasks);

    // Priority Scheduling
    cout << "Priority Scheduling:\n";
    
    PriorityScheduling ps(tm);
    ps.Schedule();
    ps.CalculateMetrics();
    

    // Round Robin Scheduling
    cout << "Round Robin Scheduling:\n";
   
    RoundRobin rf(tm, 2);
    rf.Schedule();
    rf.CalculateMetrics();
   

    // Shortest Job First Scheduling
    cout << "Shortest Job First Scheduling:\n";
   
    ShortestJobFirst sjf(tm);
    sjf.Schedule();
    sjf.CalculateMetrics();
    
    for (auto task : tasks)
    {
        delete task;
    }

    return 0;
}
