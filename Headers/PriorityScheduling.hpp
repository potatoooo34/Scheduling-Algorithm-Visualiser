#ifndef PRIORITYSCHEDULING_HPP
#define PRIORITYSCHEDULING_HPP

#include "Scheduler.hpp"
#include "TaskManager.hpp"
#include "Compare.hpp"
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
