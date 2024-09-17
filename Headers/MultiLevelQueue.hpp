#ifndef MULTILEVELQUEUE_HPP
#define MULTILEVELQUEUE_HPP

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
