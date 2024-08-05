#include "MyLock.h"

MyLock::MyLock(){
    if(pthread_mutex_init(&my_lock_, NULL) != 0)
    {
        perror("mutex init!");
        exit(-1);
    }
}

MyLock::~MyLock(){
    if(pthread_mutex_destroy(&my_lock_) != 0)
    {
        perror("mutex destroy!");
        exit(-1);
    }
}

bool MyLock::lock()
{
    return pthread_mutex_lock(&my_lock_);
}

bool MyLock::unlock()
{
    return pthread_mutex_unlock(&my_lock_);
}