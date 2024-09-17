#include "headers/TaskManager.hpp"

#include<vector>
using namespace std;

TaskManager::TaskManager(std::vector<Task *> taskslist) : tasks(taskslist) {}
TaskManager::~TaskManager() {
    for (auto task : tasks) {
        delete task;
    }
}

void TaskManager::addTask(Task *task) { tasks.push_back(task); }
vector<Task *> &TaskManager::getTasks() { 
    return this->tasks; 
}


Task *TaskManager::getTaskById(int id) {
    for (auto task : tasks) {
        if (task->getPid() == id) {
            return task;
        }
    }
    return nullptr;
}
