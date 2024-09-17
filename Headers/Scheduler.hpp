#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

class Scheduler
{
public:
    virtual void Schedule() = 0;
    virtual void CalculateMetrics() = 0;
    virtual ~Scheduler() {}
};

#endif
