#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// 先读取，再写入
int main()
{
    // 查询管道1和2
    int exsist = access("ff1", F_OK);
    int ret;
    if(exsist == 0) // 如果已经存在
    {
        printf("ff1 exsist\n");
    }
    else // 不存在则先创建管道文件
    {
        printf("not exsist, creating...\n");
        ret = mkfifo("ff1", 0764); 
        if(ret == -1)
        {
            perror("mkfifo");
            exit(-1);
        }
    }
    exsist = access("ff2", F_OK);
    if(exsist == 0) // 如果已经存在
    {
        printf("ff2 exsist\n");
    }
    else // 不存在则先创建管道文件
    {
        printf("not exsist, creating...\n");
        ret = mkfifo("ff2", 0764); 
        if(ret == -1)
        {
            perror("mkfifo");
            exit(-1);
        }
    }


    // 打开管道
    int reto1 = open("ff1", O_RDONLY);
    if(reto1 == -1)
    {
        perror("ff1 open");
        exit(-1);
    }
    printf("ff1 ready for receiving...\n");

    int reto2 = open("ff2", O_WRONLY);
    if(reto2 == -1)
    {
        perror("ff2 open");
        exit(-1);
    }
    printf("ff2 ready for sending...\n");


    // 开始从管道1 接收数据，在管道2 发送数据
    int i = 0;
    char buf[1024];
    while(1)
    {
        // 读管道1
        // memset(buf, 0, 1024);
        ret = read(reto1, buf, 1024);
        if(ret <= 0)
        {
            perror("read");
            exit(-1);
        }
        printf("what B receive: %s\n", buf);


        memset(buf, 0, 1024);
        // sprintf(buf, " here is %d", i++);
        fgets(buf, 1024, stdin);
        ret = write(reto2, buf, 1024);
        printf("B send :%s\n", buf);
        if(ret == -1)
        {
            perror("write");
            exit(-1);
        }
        // sleep(5);
    }
    close(reto1);
    close(reto2);
    return 0;
}