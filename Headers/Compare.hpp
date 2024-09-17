#ifndef COMPARE_H
#define COMPARE_H

#include "Task.hpp"

class Compare
{
public:
    bool operator()(Task *a, Task *b);
};

#endif
