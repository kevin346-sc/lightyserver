#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void* myroutine(void* i)
{
    printf("new thread!, I am %d\n", *(int*)(i));
    printf("my id: %ld\n", pthread_self());
    sleep(1);
    printf("done\n");
    return NULL;
}
int main()
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_t pth[5];
    int ret;
    for(int i = 0; i < 5; i++)
    {
        ret = pthread_create(&pth[i],&attr,myroutine, &i);
        if(ret != 0)
        {
            char * err = strerror(ret);
            printf("%s",err);
        }
        sleep(1);
    }

    printf("main thread done...\n");


    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
    return 0;
}