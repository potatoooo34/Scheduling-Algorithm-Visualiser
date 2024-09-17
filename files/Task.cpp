#include "headers/Task.hpp"

Task::Task(int pid, int at, int bt, int pr)
    : pid(pid), arrival_time(at), burst_time(bt), rem_time(bt), priority(pr), wait_time(0), finished(false) {}

int Task::getRemTime() const { return rem_time; }
int Task::getPriority() const { return priority; }
int Task::getPid() const { return pid; }
int Task::getArrivalTime() const { return arrival_time; }
int Task::getBurstTime() const { return burst_time; }
void Task::decreaseRemTime(int time) { rem_time -= time; }
void Task::setRemTime(int time) { rem_time = time; }
void Task::increasePriority(int change) { priority += change; }
int Task::getWaitTime() const { return wait_time; }
bool Task::getFinished() const { return finished; }
void Task::setFinished(bool cond) { finished = cond; }
void Task::setWaitingTime(int time) { wait_time = time; }
