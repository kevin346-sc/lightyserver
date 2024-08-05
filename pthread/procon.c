#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

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

// 生产者
void* producer(void* v)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        struct node* newnode = (struct node*)malloc(sizeof(struct node));
        newnode->val = num++;
        newnode->next = head;
        head = newnode;
        printf("producer produce :%d, id :%ld\n", head->val, pthread_self());
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
        pthread_mutex_lock(&mutex);
        struct node* tmp = head;
        if(head != NULL)
        {
            printf("consumer consume :%d, id :%ld\n", head->val, pthread_self());
            num--;
            head = head->next;
            free(tmp);
            pthread_mutex_unlock(&mutex);
            usleep(600);
        }
        else
            pthread_mutex_unlock(&mutex);

    }
    return NULL;
}


/// @brief 用mutex实现生产者，消费者模型，头插法实现仓库的货物增加
/// @bug 不加上mutex锁会段错误；锁必须要在消费者进入判断前加上，否则发生段错误；加上mutex在仓库为空时不释放锁，会发生死锁；mutex会持续占用cpu检查是否得到锁，效率低
int main()
{
    pthread_mutex_init(&mutex, NULL);

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
    pthread_exit(NULL);

    return 0;
}