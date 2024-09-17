#ifndef SHORTESTJOBFIRST_HPP
#define SHORTESTJOBFIRST_HPP

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
