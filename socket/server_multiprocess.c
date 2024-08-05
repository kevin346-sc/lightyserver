#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <errno.h>

void myhandler(int arg)
{
    while(1)
    {
        int res = waitpid(-1, NULL, WNOHANG);
        if(res == -1)
            break;
        else if(res == 0)
            continue;
        else
            printf("child %d died...\n", res);
    }
}
int main()
{
    // 注册信号回收子进程
    struct sigaction act;
    act.sa_handler = myhandler;
    sigset_t st;
    sigemptyset(&st);
    act.sa_mask = st;
    act.sa_flags = 0;
    int ret = sigaction(SIGCHLD, &act, NULL);
    if(ret == -1)
    {
        perror("sigaction");
        exit(-1);
    }
    
    // 创建套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
        perror("socket");
        exit(-1);
    }

    // 套接字与端口，ip绑定
    struct sockaddr_in addr;
    char *cp = "192.168.50.130";
    inet_pton(AF_INET, cp, &addr.sin_addr.s_addr);
    addr.sin_port = htons(9999);
    addr.sin_family = AF_INET;
    ret = bind(fd, (struct sockaddr*)&addr, sizeof addr);
    if(ret == -1)
    {
        perror("bind");
        exit(-1);
    }


    // 监听
    ret = listen(fd, 8);
    if(ret == -1)
    {
        perror("listen");
        exit(-1);
    }

    // 循环等待连接
    while(1)
    {
        struct sockaddr_in addr_client;
        int len = sizeof(addr_client);
        int ffd = accept(fd, (struct sockaddr*)& addr_client, &len);
        if(ffd == -1)
        {
            if(errno == EINTR)
                continue;
            perror("accept");
            exit(-1);
        }


        pid_t pid = fork();

        if(pid == 0)// 子进程
        {
            // 打印客户端信息
            char buf[1024] = {0};
            inet_ntop(AF_INET, &addr_client.sin_addr.s_addr, buf, sizeof buf);
            printf("connect client ip: %s, port: %d\n", buf, ntohs(addr_client.sin_port));

            char buff[1024] = {0};
            while(1)
            {
                ret = read(ffd, buff, sizeof(buff));
                if(ret > 0)
                    printf("server receive: %s\n", buff);
                else if(ret == 0)
                {
                    printf("lose connection...\n");
                    break;
                }
                else if(ret == -1)
                {
                    perror("read");
                    exit(-1);
                }
                write(ffd, buff, sizeof(buff));
            }
            close(ffd);
            exit(0);
        }
    }
    close(fd);

    return 0;
}