// 用mmap进行文件复制
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int fd1 = open("english.txt", O_RDONLY);
    if(fd1 == -1)
    {
        perror("open");
        exit(-1);
    }

    int leng = lseek(fd1, 0, SEEK_END);

    int fd2 = open("cpy.txt", O_RDWR | O_CREAT, 0664);
    if(fd2 == -1)
    {
        perror("open");
        exit(-1);
    }
    lseek(fd2, leng, SEEK_END);
    write(fd2, " ", 1);
    // truncate("cpy.txt", leng);


    void* ptr1 = mmap(NULL, leng, PROT_READ, MAP_SHARED, fd1, 0);
    void* ptr2 = mmap(NULL, leng, PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
    if(ptr1 == MAP_FAILED)
    {
        perror("mmap");
        exit(-1);
    }
    if(ptr2 == MAP_FAILED)
    {
        perror("mmap");
        exit(-1);
    }
    memcpy(ptr2, ptr1, leng);

    munmap(ptr2, leng);
    munmap(ptr1, leng);

    close(fd2);
    close(fd1);
    return 0;
}