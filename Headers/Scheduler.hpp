#ifndef SCHEDULER_H
#define SCHEDULER_H

class Scheduler
{
public:
    virtual void Schedule() = 0;
    virtual void CalculateMetrics() = 0;
    virtual ~Scheduler() {}
};

#endif
