#ifndef _THREAD_POOL_
#define _THREAD_POOL_

#include <pthread.h>
#include <queue>
#include <iostream>
#include <assert.h>

using namespace std;

class ThreadPool;

class Task
{
public:
    Task(void* param):m_param(param){}
    virtual ~Task(){}
    virtual void Run() = 0;
protected:
    void* m_param;
};
/*
** A Wrapper class of condition variable & mutex
*/
class TaskNotify
{
public:
    TaskNotify(){
        pthread_mutex_init(&m_mutex,NULL);
        pthread_cond_init(&m_cond,NULL);
    }
    ~TaskNotify(){
        pthread_mutex_destroy(&m_mutex);
        pthread_cond_destroy(&m_cond);
    }
    void Lock(){pthread_mutex_lock(&m_mutex);}
    void Unlock(){pthread_mutex_unlock(&m_mutex);}
    void Notify(){pthread_cond_signal(&m_cond);}
    void NotifiAll(){pthread_cond_broadcast(&m_cond);}
    void Wait(){pthread_cond_wait(&m_cond,&m_mutex);}
private:
    pthread_mutex_t m_mutex;
    pthread_cond_t  m_cond;
};

/*
** WorkerThread.used to fetch task from task queue 
** and execute the task
*/

class WorkerThread
{
public:
    WorkerThread();
    ~WorkerThread();
    void SetThreadPool(ThreadPool* pool){m_thread_pool = pool;}
    void StartThread(int idx);
    static void* ThreadProc(void *);
    pthread_t GetThreadId(){return m_thread_id;}
    void RunTask();
private:
    pthread_t    m_thread_id;
    unsigned int m_idx;
    /*
    * m_thread_pool stores the pool object. we need the pool object
    * because we need to fetch task from task queue,which is a member
    * of the pool object
    */
    ThreadPool* m_thread_pool;
};

class ThreadPool
{
public:
    ThreadPool(int poolsize,bool bLinger = false);
    ~ThreadPool();
    int GetPoolSize(){return m_pool_size;}
    void PushTask(Task* task);
    Task* PopTask();
    void PoolStart();
    void PoolStop();
    pthread_t GetThreadId(int idx);
private:
    queue<Task*>   m_task_queue;
    WorkerThread*  m_thread_pool;
    int            m_pool_size;
    TaskNotify     m_notifier;
    bool           m_bRunning;
    /*
    **When m_bLinger is false, A call to PoolStop will cause the 
    **thread stop fetch task from task queue and then stop.
    **When m_bLinger is true,thread will still fetch task from task 
    **queue until the task queue become empty.
    */
    bool           m_bLinger;
};

#endif/*_THREAD_POOL_*/
