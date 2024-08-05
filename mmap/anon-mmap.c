#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#define _DEFAULT_SOURCE 1

// 匿名内存映射_无实体文件

int main()
{
    int len = 1024;
    void * ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if(ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(0);
    }

    pid_t pid = fork();
    if(pid > 0)
    {
        strcpy(ptr, "hahah I am daddy!");
        wait(NULL);
    }
    else if(pid == 0)
    {
        char buf[1024] = {0};
        sprintf(buf, "%s", (char *)ptr);
        printf("%s", buf);
    }
    munmap(ptr, len);
    return 0;
}