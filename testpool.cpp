#include "threadpool.h"
#include <iostream>
#include <unistd.h>
using namespace std;

class DemoTask : public Task
{
public:
    DemoTask(){}
    ~DemoTask(){}
    void Run(){
        sleep(1);
        cout<<"Thread "<<pthread_self()<<"Execute task" <<endl;
    }
};

int main(int argc, char const *argv[])
{
    ThreadPool *pPool = new ThreadPool(5,true);
    pPool->PoolStart();

    for (int i = 0; i < 30; ++i)
    {
        pPool->PushTask(new DemoTask());
    }
 
    pPool->PoolStop();

    for (int i = 0; i < pPool->GetPoolSize(); ++i)
    {
        pthread_t tid = pPool->GetThreadId(i);
        pthread_join(tid,NULL);
    }    
    return 0;
}
