#include "threadpool.h"

WorkerThread::WorkerThread(){}
WorkerThread::~WorkerThread(){}

void WorkerThread::StartThread(int idx)
{
    pthread_create(&m_thread_id,NULL,ThreadProc,this);
    pthread_detach(m_thread_id);
    m_idx = idx;
}

void* WorkerThread::ThreadProc(void* arg)
{
    WorkerThread* pWorker = (WorkerThread*)arg;
    if (pWorker){
        pWorker->RunTask();
    }
    return NULL;
}

void WorkerThread::RunTask()
{
    while (true)
    {
        Task* pTask = m_thread_pool->PopTask();
        if (!pTask)
            break;
        //cout<<"Thread "<<m_idx<<" Running Task "<<endl;
        pTask->Run();
    }
}

ThreadPool::ThreadPool(int poolsize, bool bLinger):
        m_pool_size(poolsize),
        m_bRunning(false),
        m_bLinger(bLinger)
{
    if (m_pool_size > 0){
        m_thread_pool = new WorkerThread[m_pool_size];
    }
    for (int i = 0; i < m_pool_size; ++i){
        m_thread_pool[i].SetThreadPool(this);
    }
}

ThreadPool::~ThreadPool()
{
    delete [] m_thread_pool;
    
    while(!m_task_queue.empty()){
        Task* pTask = m_task_queue.front();
        m_task_queue.pop();
        delete pTask;
    }
}

void ThreadPool::PushTask(Task* pTask)
{
    m_notifier.Lock();

    if (m_bRunning){
        m_task_queue.push(pTask);
        m_notifier.Notify();
    }else{
        cout<<"Pool has been stopped! Could not add new task ..."<<endl;
    }

    m_notifier.Unlock();
}

Task* ThreadPool::PopTask()
{
    m_notifier.Lock();

    Task* pTaskRet = NULL;

    if (m_bLinger)
    {
        while(m_bRunning && m_task_queue.empty()){
            m_notifier.Wait();
        }
        if (!m_task_queue.empty()){
            pTaskRet = m_task_queue.front();
            m_task_queue.pop();
        }
    }
    else
    {
        while (m_bRunning && m_task_queue.empty()){
            m_notifier.Wait();
        }    

        if (m_bRunning){
            pTaskRet = m_task_queue.front();
            m_task_queue.pop(); 
        }
    }
    m_notifier.Unlock();
    return pTaskRet;
}

pthread_t ThreadPool::GetThreadId(int idx)
{
    assert(idx >=0 && idx < m_pool_size);
    return m_thread_pool[idx].GetThreadId();
}

void ThreadPool::PoolStart()
{
    if (!m_bRunning){
        m_bRunning = true;
        for (int i = 0; i < m_pool_size; ++i){
            m_thread_pool[i].StartThread(i+1);
        }
        cout<<"thread pool started ..."<<endl;
    }else{
        cout<<"thread pool already started ..."<<endl;
    }
}

void ThreadPool::PoolStop()
{
    if (m_bRunning){
        m_bRunning = false;
        m_notifier.NotifiAll();
        cout<<"thread pool is stopping......."<<endl;
    }else{
        cout<<"thread pool already stopped ......."<<endl;
    }
}
