#ifndef LOCKER_H
#define LOCKER_H

#include <pthread.h>
#include <exception>
#include <semaphore.h>
/*自定义locker类，包装mutex
接口：
lock(thread_mutex_t& m_mutex) 给线程池等待队列thread_pool上锁
unlock(thread_mutex_t& m_mutex) 解锁
*/
class locker
{
private:
    /* data */
    pthread_mutex_t m_mutex;
public:
    locker(/* args */);
    ~locker();
    bool lock();
    bool unlock();
};

locker::locker(/* args */)
{
    if(pthread_mutex_init(&m_mutex, NULL) != 0)
    {
        throw std::exception();
    }
}

locker::~locker()
{
    pthread_mutex_destroy(&m_mutex);
}
bool locker::lock()
{
    return pthread_mutex_lock(&m_mutex) == 0;
}

bool locker::unlock()
{
    return pthread_mutex_unlock(&m_mutex) == 0;
}

// 自定义条件变量类，与locker一起维护线程池请求队列线程同步
class cond
{
private:
    /* data */
    pthread_cond_t m_cond;
public:
    cond(/* args */);
    ~cond();
    // 等待其他线程释放锁
    bool wait(pthread_mutex_t* m_mutex);
    // 等待其他线程释放锁或固定时间后获取锁
    bool timedwait(pthread_mutex_t* m_mutex, struct timespec timesp);
    // 发送信号唤醒一个线程可以获取锁
    bool signal();
    // 广播信号唤醒所有线程获取锁
    bool broadcast();
};

cond::cond(/* args */)
{
    if(pthread_cond_init(&m_cond, NULL)!= 0)
    {
        throw std::exception();
    }
}

cond::~cond()
{
    pthread_cond_destroy(&m_cond);
}

bool cond::wait(pthread_mutex_t* m_mutex)
{
    return pthread_cond_wait(&m_cond, m_mutex) == 0;
}

bool cond::timedwait(pthread_mutex_t* m_mutex, struct timespec timesp)
{
    return pthread_cond_timedwait(&m_cond, m_mutex, &timesp) == 0;
}

bool cond::signal()
{
    return pthread_cond_signal(&m_cond) == 0;
}

bool cond::broadcast()
{
    return pthread_cond_broadcast(&m_cond) == 0;
}

class sem
{
private:
    /* data */
    sem_t m_sem;
public:
    sem();
    sem(int n);
    ~sem();
    bool wait();
    bool post();
};

sem::sem()
{
    if(sem_init(&m_sem, 0, 0) != 0)
    {
        throw std::exception();
    }
}

sem::sem(int n)
{
    if(sem_init(&m_sem, 0, n) != 0)
    {
        throw std::exception();
    }
}

sem::~sem()
{
    sem_destroy(&m_sem);
}

bool sem::post()
{
    return sem_post(&m_sem) == 0;
}

bool sem::wait()
{
    return sem_wait(&m_sem) == 0;
}
#endif