#include "../Headers/ShortestJobFirst.hpp"

#include <iostream>
#include <vector>
using namespace std;

ShortestJobFirst ::ShortestJobFirst(TaskManager &tm) : tm(tm) {}

void ShortestJobFirst ::Schedule()
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
