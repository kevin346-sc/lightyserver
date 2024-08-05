#include <poll.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/epoll.h>

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

    // 注册epoll
    int epfd = epoll_create(1);
    if(epfd == -1)
    {
        perror("epoll_create");
        exit(-1);
    }

    struct epoll_event epollevent;
    epollevent.events = EPOLLIN;
    epollevent.data.fd = fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &epollevent);

    struct epoll_event events[1024];

    while(1)
    {
        int ret = epoll_wait(epfd, events, 1024, -1);
        if (ret == -1)
        {
            perror("epoll_wait");
            exit(-1);
        }

        for(int i = 0; i < ret; i++)
        {
            if(fd == events[i].data.fd) // 之前监听的文件描述符epfd发生了变化，意味着有新连接需要建立
            {
                struct sockaddr_in addr_client;
                socklen_t len = sizeof(addr_client);
                int ffd = accept(fd, (struct sockaddr*)& addr_client, &len);

                if(ffd == -1)
                {
                    perror("accept");
                    exit(-1);
                }
                // 将新建立的文件描述符，即对应一个客户端，加入epoll监听
                epollevent.data.fd = ffd;
                epollevent.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_ADD, ffd, &epollevent);
            }
            else// 监听的有连接的文件描述符有变化，即收到客户端发送的数据
            {

                char buf[1024]={0};
                int len = read(events[i].data.fd, buf, sizeof(buf));
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
                    write(events[i].data.fd, buf, sizeof(buf));
                }
            }
        }

    }
    close(fd);
    return 0;
}