#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "locker.h"
#include <list>
#include <exception>
#include <cstdio>

// T 是任务类
template<typename T>
class thread_pool
{
private:
    // 线程池中的线程个数
    int m_thread_number;
    // 描述线程池的线程数组，大小与线程个数相等
    pthread_t * m_threads;

    // 线程池能够接受的最大请求数量
    int m_mrequests;
    // 线程池请求队列，用链表表示
    std::list<T*> m_request_queue;

    // 维护线程池互斥的locker类
    locker m_locker;
    // 维护线程池同步的cond类
    cond m_cond;
    // 维护线程池同步的sem类
    sem m_sem;

    // 结束线程的标志
    bool m_kill;

    // 线程运行函数，所有对象共用一个函数，循环从请求队列中取出任务并执行
    static void* worker(void* argv);
public:
    thread_pool(int m_thread_number = 8, int m_mrequests = 1000);
    ~thread_pool();
    bool append(T* request);
};

template<typename T>
thread_pool<T>::thread_pool(int thread_number, int mrequests):
    m_thread_number(thread_number),
    m_mrequests(mrequests),
    m_kill(false),
    m_threads(NULL)
{
    if(thread_number <= 0 || mrequests <= 0)
    {
        throw std::exception();
    }
    // 为线程池中的线程分配内存空间
    m_threads = new pthread_t[m_thread_number];
    printf("initializing...");
    // 在对应的内存空间上创建线程
    for(int i = 0; i < m_thread_number; i++)
    {
        if(pthread_create(&m_threads[i], NULL, worker, this) != 0)
        {
            delete [] m_threads;
            throw std::exception();
        }
        if(pthread_detach(m_threads[i]) != 0)
        {
            delete [] m_threads;
            throw std::exception();
        }
    }
}

template<typename T>
thread_pool<T>::~thread_pool()
{
    m_kill = true;
    delete[] m_threads;
}

template<typename T>
bool thread_pool<T>::append(T* request)
{
    // 先请求锁
    if(!m_locker.lock())
    {
        throw std::exception();
    }
    // 已经超过最大支持请求
    if(m_request_queue.size() >= m_mrequests)
    {
        m_locker.unlock();
        return false;
    }

    m_request_queue.push_back(request);
    m_sem.post(); // 通知工作线程有活干了
    m_locker.unlock();
}

template<typename T>
void* thread_pool<T>::worker(void* argv)
{
    printf("pthread %d created and ready\n", pthread_self());
    // 创建线程时
    thread_pool* pool = (thread_pool *) argv;
    // 发送信号，等待request到来，阻塞
    pool->m_sem.wait();
    pool->m_locker.lock();
    // 一旦唤醒该线程，取出工作队列中第一个任务，并执行request的process工作函数
    pool->m_request_queue.front()->process();
    pool->m_request_queue.pop_front();
    pool->m_locker.unlock();
}

#endif