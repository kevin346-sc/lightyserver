#ifndef MYSEM_H
#define MYSEM_H

#include <semaphore.h>
class MySem
{
private:
    sem_t my_sem_;
public:
    MySem();
    MySem(int t);
    ~MySem();
    bool wait();
    bool post();
};



#endif