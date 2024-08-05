// mmap 实现父子进程通信
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

int main ()
{
    int ret = access("hello.txt", F_OK);
    int retopen;
    if(ret == -1)
    {
        printf("not exist, creating new and opening....\n");
        retopen = open("hello.txt", O_RDWR | O_CREAT,0664);
        if(retopen == -1)
        {
            perror("open");
            exit(-1);
        }
        char* buf = "hi, here is father!!";
        write(retopen, buf, strlen(buf));
    }
    else
    {
        printf("existed, opening...\n");
        retopen = open("hello.txt", O_RDWR);
        if(retopen == -1)
        {
            perror("open");
            exit(-1);
        }
    }
    int length = lseek(retopen, 0, SEEK_END);
    void* ptr = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, retopen, 0);
    
    char buf[128] = {0};
    ret = fork();
    if(ret == -1)
    {
        perror("fork");
        exit(-1);
    }
    else if(ret > 0)
    {
        //父进程，接收信息
        wait(NULL);
        printf("parent, receiving..\n");
        memcpy(buf, ptr, sizeof(buf));
        printf("parent receive: %s\n", buf);
    }
    else{
        //子进程，改动信息
        printf("child, sending..\n");
        memcpy(ptr, "hi, here is son!!hahah", 23);
    }
    munmap(ptr, length);
    return 0;
}