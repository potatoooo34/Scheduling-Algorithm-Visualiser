#include "headers/PriorityScheduling.hpp"
#include "Headers/Compare.hpp"

#include <queue>
#include <iostream>
using namespace std;

PriorityScheduling::PriorityScheduling(TaskManager &tm) : taskmanager(tm)
{
    int n = taskmanager.getTasks().size();
    arrive.resize(n, 0.0);
    finish.resize(n, 0.0);
}

void PriorityScheduling::Schedule()
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

    
