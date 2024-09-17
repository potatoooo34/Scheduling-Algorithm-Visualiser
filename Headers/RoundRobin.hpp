#ifndef ROUNDROBIN_H
#define ROUNDROBIN_H

#include <vector>
#include <queue>
#include "Scheduler.hpp"
#include "TaskManager.hpp"

class RoundRobin : public Scheduler
{
private:
    TaskManager &tm;
    int timequantum;
    std::vector<double> arrival;
    std::vector<double> finish;

public:
    RoundRobin(TaskManager &tm, int tq);
    void Schedule() override;
    void CalculateMetrics() override;
};

#endif
