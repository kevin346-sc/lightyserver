#include <unistd.h>
#include <stdio.h>
#include<string.h>
#include<strings.h>
#include <stdlib.h>
#include <wait.h>
#include <fcntl.h>
#define _GNU_SOURCE

// ps aux | grep xxx
int main(int argc, char * argv[])
{
    if(argc < 2)
    {
        perror("too few arguments");
        return -1;
    }
    // 创建进程前创建管道，确保子进程的管道与父进程一致
    int pipefd[2];
    int ret = pipe(pipefd);
    if(ret == -1)
    {
        perror("pipe");
        return -1;
    }

    pid_t pid;
    pid = fork();
    if(pid > 0) 
    {
        // 父进程，从子进程接受数据，关闭写端
        printf("parent, receiving, pid:%d\n", getpid());
        // close(pipefd[1]);
        // char buf[1024];
        // read(pipefd[0], buf, sizeof(buf));
        // printf("%s", buf);

        close(pipefd[1]);
        char buf[1024];

        // 设为非阻塞
        // int flags = fcntl(pipefd[0], F_GETFL); // 获取当前状态
        // flags |= O_NONBLOCK; // 添加非阻塞状态
        // fcntl(pipefd[0]， F_SETFL, flags); // 设置状态

        while((read(pipefd[0], buf, sizeof(buf))) > 0)
        {
            printf("%s", buf);
            //memset(buf, 0, 1024);
            bzero(buf, 1024);
        }
        waitpid(-1, NULL, 0);
        wait(NULL);
        exit(0);
    }
    else if(pid == 0)
    {
        // 子进程
        printf("child, sending, pid:%d\n", getpid());
        // close(pipefd[0]);
        // char * buf = "hi, I am your son\n";
        // // char buf[1024] = "hi, I am your son\n";
        // write(pipefd[1], buf, strlen(buf));
        
        close(pipefd[0]);
        char buf[1024];
        dup2(pipefd[1], STDOUT_FILENO);
        int res = execlp("/bin/ps", "ps", "aux", NULL);
        if(res == -1)
        {
            perror("execlp");
            return -1;
        }
        exit(0);
    }
    else
    {
        perror("fork");
        return -1;
    }
    return 0;
}

int grep(char const *argv[]) {
    FILE *fp = NULL;
    int count = 0;       // 统计行
    char buf[512] = {0}; // 暂存一行数据
    char *p = NULL;

    // 每次读一行数据判断并输出
    while (fgets(buf, sizeof(buf) - 1, fp) != NULL) {
        char userName[20] = {0}; // 防止其他用户名前缀也为argv[1]
        strcpy(userName, argv[1]);
        strcat(userName, " ");
        // userName在buf中首次出现的位置在buf首地址时，匹配成功
        if ((p = strstr(buf, userName)) == buf) {
            count++;
            char outbuf[520] = {0};
            sprintf(outbuf, "%d\t%s", count, buf);
            printf("%s", outbuf);
        }
    }
    // 关闭文件
    fclose(fp);
    return 0;
}