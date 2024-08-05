#include "MyThreadpool.h"

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
    std::cout << "thread pool initialised... with "<< threads_nums_ <<"threads standing by and "<< m_requests_ <<" max requests" <<std::endl;
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
    std::cout << "thread id: " << pthread_self() << "created" << std::endl;
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