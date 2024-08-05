#ifndef MYTHREADPOOL_H
#define MYTHREADPOOL_H

#include "MyLock.h"
#include "MySem.h"
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <thread>
#include <iostream>
template <typename T>
class MyThreadpool
{
private:
    MyLock mylock;
    MySem mysem;
    uint64_t threads_nums_; // 线程池中的线程数量
    uint64_t* threads_; // 线程数组指针，

    uint64_t m_requests_; // 请求队列最大数量
    std::list<T*> request_list_; // 请求队列
    bool finish_flag_; // 结束标志

    static void* work(void* argv); // 线程工作函数，线程池共享，线程从请求队列中寻找任务
    void run();
public:
    MyThreadpool(int threads_nums = 8, int m_requests = 1000);
    ~MyThreadpool();
    bool append_request(T* request);
};

template <typename T>
MyThreadpool<T>::MyThreadpool(int threads_nums, int m_requests)
    : threads_nums_(threads_nums), threads_(NULL), m_requests_(m_requests), finish_flag_(false)
{
    if(threads_nums <= 0 || m_requests <= 0)
    {
        perror("threadpool inintial");
        exit(-1);
    }
    threads_ = new pthread_t[threads_nums_];
    for(int i = 0; i < threads_nums_; i++)
    {
        std::cout << "thread N.O. " << i << " created" << std::endl;
        if(pthread_create(&threads_[i], NULL, work, this) != 0)
        {
            perror("pthread create");
            exit(-1);
        }
        if(pthread_detach(threads_[i]) != 0)
        {
            perror("pthread detach");
            exit(-1);
        }
    }
    std::cout << "thread pool initialised... with "<< threads_nums_ <<" threads standing by and "<< m_requests_ <<" max requests" <<std::endl;
}


template <typename T>
MyThreadpool<T>::~MyThreadpool()
{
    finish_flag_ = true;
    delete[] threads_;
}

template <typename T>
bool MyThreadpool<T>::append_request(T* request)
{
    uint8_t ret = mylock.lock();
    if(ret != 0)
    {
        perror("locker lock");
        exit(-1);
    }

    if(request_list_.size() > m_requests_)
    {
        perror("too many request");
        return false;
    }
    request_list_.emplace_back(request);
    mysem.post();
    mylock.unlock();
    return true;
}

template <typename T>
void* MyThreadpool<T>::work(void* argv)
{
    MyThreadpool* pool = (MyThreadpool*)argv;
    pool->run();
    
}

template <typename T>
void MyThreadpool<T>::run()
{
    while(!finish_flag_)
    {
        mysem.wait();// 等待信号量唤醒线程
        mylock.lock();
        if(request_list_.empty())
        {
            mylock.unlock();
            continue;
        }

        T* request = request_list_.front();
        request_list_.pop_front();
        mylock.unlock();
        request->process();
    }

}

#endif