#include "headers/MultiLevelQueue.hpp"
#include <iostream>
#include<vector>
#include <vector>
using namespace std;

MultiLevelQueue::MultiLevelQueue(TaskManager &tm, int timeq) : tm(tm), timequantum(timeq) {}

void MultiLevelQueue::Schedule() {
    // Separate tasks into different queues based on priority
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
