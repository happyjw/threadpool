#include "threadpool.h"
#include <iostream>
#include <unistd.h>
using namespace std;

class DemoTask : public Task
{
public:
    DemoTask(void* param):Task(param){}
    ~DemoTask(){}
    void Run(){
        usleep(100);
        cout<<"Thread "<<pthread_self()<<" execute task "<<*(int*)m_param<<endl;
    }
};

int main(int argc, char const *argv[])
{
    ThreadPool *pPool = new ThreadPool(3,true);
    pPool->PoolStart();

    for (int i = 0; i < 30; ++i){
        pPool->PushTask(new DemoTask(new int (i+1)));
    }
 
    pPool->PoolStop(); 
    sleep(48);
    return 0;
}
