#include "MySem.h"
#include <stdio.h>
#include <stdlib.h>

MySem::MySem()
{
    if(sem_init(&my_sem_, 0, 0) != 0)
    {
        perror("sem init!");
        exit(-1);
    }
}

MySem::MySem(int t)
{
    if(sem_init(&my_sem_, 0 , t) != 0)
    {

        perror("sem t init!");
        exit(-1);
    }
}
MySem::~MySem()
{
    if(sem_destroy(&my_sem_) != 0)
    {
        perror("sem destroy!");
        exit(-1);
    }
}

bool MySem::wait()
{
    return sem_wait(&my_sem_);
}

bool MySem::post()
{
    return sem_post(&my_sem_);
}