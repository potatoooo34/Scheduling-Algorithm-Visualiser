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
    vector<Task*> taskscopy;
    int timequantum;
    vector<double> arrival;
    vector<double> finish;

public:
    RoundRobin(vector<Task*> tasks, int tq)
    {
        this->taskscopy = tasks;
        this->timequantum = tq;
        int n = tasks.size();
        arrival.resize(n,0.0);
        finish.resize(n, 0.0);
        
    }

    void Schedule()
    {
        vector<Task*> tasks = taskscopy;
        int n = tasks.size();
        
        sort(tasks.begin(), tasks.end(), [](Task *a, Task *b)
             { return a->arrival_time < b->arrival_time; });
        queue<Task *> q;
        
        int curr_time = 0;
        int i = 0;
        

        while (!q.empty() || i < n)
        {
            while (i < n && tasks[i]->arrival_time <= curr_time)
            {
                arrival[tasks[i]->pid - 1] = tasks[i]->arrival_time;
                q.push(tasks[i]);
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
                    finish[t->pid - 1] = curr_time;
                }
            }
            else
            {
                if (i < n)
                {
                    curr_time = tasks[i]->arrival_time;
                }
            }
        }
    }

    void avgwait(){
        int n = taskscopy.size();
        double sum = 0;
        for(int i =0 ; i < n ; i++){
            sum += finish[i] - arrival[i];
        }

        cout<<"Avg turnaround time: "<<sum / double(n)<<endl;
    }

    
};

class ShortestJobFirst
{
    vector<Task*> taskscopy;

public:
    ShortestJobFirst(vector<Task*> tasks)
    {
        this->taskscopy = tasks;
    }

    void schedule()
    {
        vector<Task*> tasks = taskscopy;
        sort(tasks.begin(), tasks.end(), [](Task *a, Task *b)
             { return a->burst_time < b->burst_time; });
        int curr_time = 0;
        for (auto &it : tasks)
        {
            curr_time = max(curr_time, it->arrival_time);
            cout << "Process id " << it->pid
                 << " Executed For " << it->burst_time << " ms. Current Time " << curr_time << endl;
            curr_time += it->burst_time;
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
    vector<Task *> taskscopy;

public:
    PriorityScheduling(vector<Task *> tasks)
    {
        this->taskscopy = tasks;
    }

    void Schedule()
    {
        vector<Task*> tasks =this->taskscopy;
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
    vector<Task*> tasks = {
        new Task(1, 4, 5, 3),  // Arrives first, high priority
        new Task(2, 2, 3, 1),  // Arrives second, lowest priority
        new Task(3, 0, 8, 2),  // Arrives later, medium priority
        new Task(4, 10, 2, 4),  // Arrives after some tasks, higher priority
        new Task(5, 6, 4, 2),  // Medium priority, arrives when others are running
        new Task(6, 11, 6, 5), // Arrives late, lowest priority
        new Task(7, 12, 7, 1), // Arrives when system is busy, highest priority
        new Task(8, 10, 3, 3), // Arrives late, medium priority
        new Task(9, 13, 5, 4), // Arrives after many, higher priority
        new Task(10, 18, 4, 2) // Arrives near end, medium priority
    };


    // PriorityScheduling ps(tasks);
    // ps.Schedule();

    RoundRobin rf(tasks , 2);
    rf.Schedule();
    rf.avgwait();

    ShortestJobFirst sjf(tasks);
    sjf.schedule();

   
    for (auto task : tasks)
    {
        delete task;
    }

    return 0;
}
