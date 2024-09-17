#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <vector>
#include "Task.hpp"
#include<stdexcept>

class TaskManager
{
private:
    std::vector<Task *> tasks;

public:
    TaskManager(std::vector<Task *> taskslist);
    ~TaskManager();
    void addTask(Task *task);
    std::vector<Task *> &getTasks();
    Task *getTaskById(int id);
};

#endif
