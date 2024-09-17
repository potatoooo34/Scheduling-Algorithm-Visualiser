#ifndef SHORTESTJOBFIRST_H
#define SHORTESTJOBFIRST_H

#include "TaskManager.hpp"
#include "Scheduler.hpp"

class ShortestJobFirst : public Scheduler
{
private:
    TaskManager &tm;

public:
    ShortestJobFirst(TaskManager &tm);
    void Schedule() override;
};

#endif
