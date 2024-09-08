#include <iostream>
#include <string>
#include <vector>
#include <queue>

using namespace std;
// create a class with pid , burst time and arrival time , priority

class Task
{
public:
    int pid;
    int arrival_time;
    int burst_time;
    int rem_time;
    int priority;
    Task(int pid, int at, int bt, int pr)
    {
        this->pid = pid;
        this->arrival_time = at;
        this->burst_time = bt;
        this->rem_time = bt;
        this->priority = pr;
    }
};
class RoundRobin
{
    vector<Task> taskss;
    int timequantum;

public:
    RoundRobin(vector<Task> tasks, int tq)
    {
        this->taskss = tasks;
        this->timequantum = tq;
    }

    void Schedule()
    {
        vector<Task> tasks = taskss;
        sort(tasks.begin(), tasks.end(), [](Task &a, Task &b)
             { return a.arrival_time < b.arrival_time; });
        queue<Task *> q;

        int curr_time = 0;
        int i = 0;
        int n = tasks.size();

        while (!q.empty() || i < n)
        {
            while (i < n && tasks[i].arrival_time <= curr_time)
            {
                q.push(&tasks[i]);
                i++;
            }

            if (!q.empty())
            {
                Task *t = q.front();
                q.pop();

                int time_executed = min(t->rem_time, this->timequantum);
                curr_time += time_executed;
                t->rem_time -= time_executed;
                cout << "Process id " << t->pid << " Executed for " << time_executed
                     << " ms Remaining time " << t->rem_time << ". Current Time " << curr_time << endl;
                if (t->rem_time > 0)
                {
                    q.push(t);
                }
                else
                {
                    cout << "Process id " << t->pid << " Finished at " << curr_time << endl;
                }
            }
            else
            {
                if (i < n)
                {
                    curr_time = tasks[i].arrival_time;
                }
            }
        }
    }
};

class ShortestJobFirst
{
    vector<Task> tasks;

public:
    ShortestJobFirst(vector<Task> tasks)
    {
        this->tasks = tasks;
    }

    void schedule()
    {
        sort(tasks.begin(), tasks.end(), [](Task &a, Task &b)
             { return a.burst_time < b.burst_time; });
        int curr_time = 0;
        for (auto &it : tasks)
        {
            curr_time = max(curr_time, it.arrival_time);
            cout << "Process id " << it.pid
                 << " Executed For " << it.burst_time << " ms. Current Time " << curr_time << endl;
            curr_time += it.burst_time;
        }
    }
};
//lower number higher priority
class PriorityScheduling
{
    vector<Task*> tasks;

public:
    PriorityScheduling(vector<Task*> tasks)
    {
        this->tasks = tasks;
    }

    void Schedule()
    {
        
    }
};

int main()
{
    vector<Task> tasks = {
        Task(1, 0, 10, 1),
        Task(2, 3, 5, 2),
        Task(3, 7, 8, 3),
        Task(4, 12, 2, 4),
        Task(5, 15, 6, 5),
        Task(6, 20, 12, 1),
        Task(7, 25, 3, 2),
        Task(8, 28, 7, 3),
        Task(9, 33, 4, 4),
        Task(10, 40, 11, 5),
        Task(11, 42, 9, 1),
        Task(12, 45, 15, 2),
        Task(13, 50, 2, 3),
        Task(14, 55, 6, 4),
        Task(15, 60, 1, 5),
        Task(16, 65, 10, 1),
        Task(17, 70, 5, 2),
        Task(18, 75, 3, 3),
        Task(19, 80, 8, 4),
        Task(20, 90, 7, 5)};

    cout << "Shortest Job First Scheduling Simulation:" << endl;
    ShortestJobFirst sjf(tasks);
    sjf.schedule();

    return 0;
}
