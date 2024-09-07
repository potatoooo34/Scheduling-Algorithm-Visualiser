#include<iostream>
#include<string>
#include<vector>
#include<queue>

using namespace std;
//create a class with pid , burst time and arrival time

class Task{
    public:
        int pid;
        int arrival_time;
        int burst_time;
        int rem_time;
        Task(int pid , int at , int bt){
            this->pid = pid;
            this->arrival_time = at;
            this->burst_time = bt;
            this->rem_time = bt;
        }
};
class RoundRobin {
    vector<Task> taskss;
    int timequantum;
public:
    RoundRobin(vector<Task> tasks , int tq){
        this->taskss = tasks;
        this->timequantum = tq;
    }

    void Schedule(){
        vector<Task> tasks = taskss;
        sort(tasks.begin(), tasks.end(), [](Task &a, Task &b) {
            return a.arrival_time < b.arrival_time;
        });
        queue<Task*> q;

        int curr_time = 0;
        int i = 0;
        int n = tasks.size();

        while(!q.empty() || i < n){
            while( i  < n && tasks[i].arrival_time <= curr_time){
                q.push(&tasks[i]);
                i++;
            }

            if(!q.empty()){
                Task* t = q.front();
                q.pop();

                int time_executed = min(t->rem_time , this->timequantum);
                curr_time += time_executed;
                t->rem_time -= time_executed;
                cout<<"Process id "<<t->pid<<" Executed for "<< time_executed 
                <<" ms Remaining time "<<t->rem_time<<". Current Time "<<curr_time<<endl;
                if(t->rem_time > 0){
                    q.push(t);
                }
                else{
                    cout<<"Process id "<<t->pid<<" Finished at "<< curr_time<<endl;
                }
            }
            else{
                if( i < n){
                    curr_time = tasks[i].arrival_time;
                }
            }
           
        }


    }
};




