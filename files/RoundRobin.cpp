#include "headers/RoundRobin.hpp"
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

RoundRobin::RoundRobin(TaskManager &tm, int tq) : tm(tm), timequantum(tq) {
        int n = tm.getTasks().size();
        arrival.resize(n, 0.0);
        finish.resize(n, 0.0);
}

void RoundRobin::Schedule() {
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

void RoundRobin::CalculateMetrics() {
    int n = tm.getTasks().size();
        double sum = 0;
        for (int i = 0; i < n; i++)
        {
            sum += finish[i] - arrival[i];
        }

        cout << "Avg turnaround time: " << sum / double(n) << endl
             << endl;
}
