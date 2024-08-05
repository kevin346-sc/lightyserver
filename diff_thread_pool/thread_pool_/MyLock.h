#ifndef MYLOCK_H
#define MYLOCK_H

#include <mutex>
#include <stdio.h>
class MyLock{
private:
    pthread_mutex_t my_lock_;

public:
    MyLock();
    ~MyLock();
    bool lock();
    bool unlock();
};
#endif