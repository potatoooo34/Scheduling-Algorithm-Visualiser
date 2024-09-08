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
class Compare
{
public:
    bool operator()(Task *a, Task *b)
    {
        return a->priority > b->priority;
    }
};
// lower number higher priority
class PriorityScheduling
{
    vector<Task *> tasks;

public:
    PriorityScheduling(vector<Task *> tasks)
    {
        this->tasks = tasks;
    }

    void Schedule()
    {
        
        priority_queue<Task *, vector<Task *>, Compare> pq;
        sort(tasks.begin(), tasks.end(), [](Task *a, Task *b)
             { return a->arrival_time < b->arrival_time; });

        int idx = 0;
        int curr_time = 0;
        int n = tasks.size();
        int time_executed = 0;
        while (!pq.empty() || idx < n)
        {
            while (idx < n && curr_time >= tasks[idx]->arrival_time)
            {
                pq.push(tasks[idx]);
                idx++;
            }
            if (!pq.empty())
            {
                auto m = pq.top();
                pq.pop();
                int time_Start = curr_time;
                while (m->rem_time > 0)
                {
                    if (idx < n && tasks[idx]->arrival_time <= curr_time && tasks[idx]->priority < m->priority)
                    {
                        break; // highr priority process arrived;
                    }
                    curr_time++;
                    m->rem_time--;
                }
                cout << "Process id " << m->pid
                     << " Executed For " << curr_time - time_Start << " ms. Current Time " << curr_time << endl;
                if (m->rem_time > 0)
                {
                    pq.push(m);
                }
                else
                {
                    cout << "Process id " << m->pid << " Finished at " << curr_time << endl;
                }
            }
            else
            {
                if (idx < n)
                    curr_time = tasks[idx]->arrival_time;
            }
        }
    }
};

int main()
{
    vector<Task *> tasks = {
        new Task(3, 9, 8, 3),
        new Task(5, 23, 6, 5),
        new Task(1, 0, 10, 1),
        new Task(4, 15, 12, 2),
        new Task(2, 7, 5, 4),
        new Task(6, 18, 9, 1),
        new Task(7, 30, 7, 3),
        new Task(8, 25, 4, 2),
        new Task(9, 40, 6, 5),
        new Task(10, 5, 11, 4)};

    PriorityScheduling ps(tasks);
    ps.Schedule();

   
    for (auto task : tasks)
    {
        delete task;
    }

    return 0;
}
