#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int count = 1;
pthread_mutex_t mutex;
pthread_rwlock_t rwlock;
//线程读操作
void* readroutine(void* num)
{
    while(1)
    {
        // pthread_mutex_lock(&mutex);
        pthread_rwlock_rdlock(&rwlock);
        printf("Read===thread: %ld, read the number: %d\n", pthread_self(), count);
        // pthread_mutex_unlock(&mutex);
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }

    return NULL;
}
//线程写操作
void* writeroutine(void* num)
{
    while(1)
    {
        // pthread_mutex_lock(&mutex);
        pthread_rwlock_wrlock(&rwlock);
        count++;
        printf("Write===thread: %ld, write the number: %d\n", pthread_self(), count);
        pthread_rwlock_unlock(&rwlock);
        // pthread_mutex_unlock(&mutex);
        usleep(100);
    }

    return NULL;
}

int main()
{
    // pthread_mutex_init(&mutex, NULL);
    pthread_rwlock_init(&rwlock, NULL);

    // 创建五个读线程，三个写线程
    pthread_t rpth[5], wpth[3];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    for(int i = 0; i < 3; i++)
    {
        pthread_create(&wpth[i], &attr, writeroutine, &i);
    }

    for(int i = 0; i < 5; i++)
    {
        pthread_create(&rpth[i], &attr, readroutine, &i);
    }
    pthread_attr_destroy(&attr);
    pthread_exit(NULL);

    // pthread_mutex_destroy(&mutex);
    pthread_rwlock_destroy(&rwlock);
    return 0;
}