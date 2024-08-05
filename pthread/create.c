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
}
int main()
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_t pth[3];
    int ret;
    for(int i = 0; i < 3; i++)
    {
        ret = pthread_create(&pth[i],&attr,myroutine, &i);
        if(ret != 0)
        {
            char * err = strerror(ret);
            printf("%s",err);
        }
        sleep(1);
    }
    for(int i = 0; i < 3; i++)
    {
        pthread_join(pth[i], NULL);
        // pthread_detach(pth[i]);
    }
    printf("main thread done...\n");
    return 0;
}