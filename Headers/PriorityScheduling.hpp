#ifndef PRIORITYSCHEDULING_H
#define PRIORITYSCHEDULING_H

#include "Scheduler.hpp"
#include "TaskManager.hpp"
#include <vector>

class PriorityScheduling : public Scheduler
{
private:
    TaskManager &taskmanager;
    std::vector<double> arrive;
    std::vector<double> finish;

public:
    PriorityScheduling(TaskManager &tm);
    void Schedule() override;
    
};

#endif
