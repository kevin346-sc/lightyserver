#include <poll.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/types.h>

int main()
{
int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    char* cp = "192.168.50.130";
    inet_pton(AF_INET, cp, &addr.sin_addr.s_addr);
    bind(fd, (struct sockaddr*)&addr, sizeof(addr));

    listen(fd, 8);

    struct pollfd pollfds[1024];
    for(int i = 0; i < 1024; i++)
    {
        pollfds[i].fd = -1;
        pollfds[i].events = POLLIN;
    }
    pollfds[0].fd = fd;
    int maxfds = 0;
    while(1)
    {
        int ret = poll(pollfds, maxfds + 1, -1);
        if(ret == -1)
        {
            perror("poll");
            exit(-1);
        }
        else if(ret == 0)
            continue;
        else if(ret > 0)// 监听到有文件描述符缓冲区数据变化
        {
            // 文件描述符fd变化代表有新客户端连接
            if(pollfds[0].revents & POLLIN)
            {
                struct sockaddr_in addr_client;
                int size = sizeof(addr_client);
                int ffd = accept(fd, (struct sockaddr*)&addr_client, &size);
                for(int i = 1; i < 1024; i++)
                {
                    if(pollfds[i].fd == -1)
                    {
                        pollfds[i].fd = ffd;
                        pollfds[i].events = POLLIN;
                        break;
                    }
                }
                maxfds = ffd > maxfds? ffd : maxfds;
            }

            for(int i = 1; i <= maxfds; i++)
            {
                // 缓冲区接受到数据
                if(pollfds[i].revents & POLLIN)
                {
                    char buf[1024]={0};
                    int len = read(pollfds[i].fd, buf, sizeof(buf));
                    if(len == -1)
                    {
                        perror("read");
                        exit(-1);
                    }
                    else if(len == 0)
                        continue;
                    else if(len > 0)
                    {
                        printf("server receive : %s\n", buf);
                        write(pollfds[i].fd, buf, sizeof(buf));
                    }
                }
            }
        }
    }
    close(fd);
    return 0;
}