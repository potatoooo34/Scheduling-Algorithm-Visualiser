#ifndef TASK_H
#define TASK_H

class Task
{
private:
    int pid;
    int arrival_time;
    int burst_time;
    int rem_time;
    int priority;
    int wait_time;
    bool finished;

public:
    Task(int pid, int at, int bt, int pr);
    int getRemTime() const;
    int getPriority() const;
    int getPid() const;
    int getArrivalTime() const;
    int getBurstTime() const;
    void decreaseRemTime(int time);
    void setRemTime(int time);
    void increasePriority(int change);
    int getWaitTime() const;
    bool getFinished() const;
    void setFinished(bool cond);
    void setWaitingTime(int time);
};

#endif
