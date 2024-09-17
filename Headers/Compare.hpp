#ifndef COMPARE_HPP
#define COMPARE_HPP

#include "Task.hpp"

class Compare
{
public:
    bool operator()(Task *a, Task *b);
};

#endif
