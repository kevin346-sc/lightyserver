#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

// 定义结点，一个结点代表一个货物
struct node{
    int val;
    struct node* next;
};

// 头结点
struct node* head = NULL;
int num = 1;

// 声明锁
pthread_mutex_t mutex;

// 声明条件变量
// pthread_cond_t cond;

// 声明信号量
sem_t psem;
sem_t csem;

// 生产者
void* producer(void* v)
{
    while(1)
    {
        sem_wait(&psem);
        pthread_mutex_lock(&mutex);
        struct node* newnode = (struct node*)malloc(sizeof(struct node));
        newnode->val = num++;
        newnode->next = head;
        head = newnode;
        printf("producer produce :%d, id :%ld\n", head->val, pthread_self());
        // pthread_cond_signal(&cond);
        sem_post(&csem);
        pthread_mutex_unlock(&mutex);
        usleep(600);
    }
    return NULL;
}

// 消费者
void* consumer(void* v)
{
    while(1)
    {
        sem_wait(&csem);
        pthread_mutex_lock(&mutex);
        struct node* tmp = head;
        printf("consumer consume :%d, id :%ld\n", head->val, pthread_self());
        num--;
        head = head->next;
        free(tmp);
        pthread_mutex_unlock(&mutex);
        sem_post(&psem);
        usleep(600);
    }
    return NULL;
}


/// @brief 用互斥锁mutex + 两个信号量psem, csem 实现生产者，消费者模型，头插法实现仓库的货物增加
/// @bug 
int main()
{
    pthread_mutex_init(&mutex, NULL);
    // pthread_cond_init(&cond, NULL);

    // 仓库大小为8
    sem_init(&psem, 0, 8);
    sem_init(&csem, 0, 0);


    // 5个生产者，5个消费者
    pthread_t cpth[5], ppth[5];
    for(int i = 0; i < 5; i++)
    {
        pthread_create(&ppth[i], NULL, producer, NULL);
        pthread_create(&cpth[i], NULL, consumer, NULL);
    }

    for(int i = 0; i < 5; i++)
    {
        pthread_detach(ppth[i]);
        pthread_detach(cpth[i]);
    }

    while(1)
    {
        sleep(10);
    }
    pthread_mutex_destroy(&mutex);
    // pthread_cond_destroy(&cond);
    sem_destroy(&psem);
    sem_destroy(&csem);

    pthread_exit(NULL);

    return 0;
}