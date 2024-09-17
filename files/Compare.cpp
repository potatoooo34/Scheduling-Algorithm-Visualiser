#include "headers/Compare.hpp"

bool Compare::operator()(Task *a, Task *b) {
    return a->getPriority() > b->getPriority(); // Lower priority value means higher priority
}
