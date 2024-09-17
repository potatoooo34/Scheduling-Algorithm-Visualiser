#ifndef MULTILEVELQUEUE_H
#define MULTILEVELQUEUE_H

#include <queue>
#include "TaskManager.hpp"
using namespace std;

class MultiLevelQueue
{
private:
    TaskManager &tm;
    queue<Task *> highest;
    queue<Task *> middle;
    queue<Task *> lower;
    int timequantum;

public:
    MultiLevelQueue(TaskManager &tm, int timeq);
    void Schedule();
};

#endif
