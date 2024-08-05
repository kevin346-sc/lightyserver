#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include "MyLock.h"
#include "MyThreadpool.h"
#include "HTTPCONN.h"
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX_FD_SZ 65535   //最大文件描述符
#define MAX_EVENTS_SZ 10000 // 最大监听数量
// 信号捕捉
void addsig(int sig, void(handler) (int))
{
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;
    sigfillset(&sa.sa_mask);
    sigaction(sig, &sa, NULL);
}

extern void addfd(int epfd, int fd, bool one_shot);
extern void delfd(int epfd, int fd);
extern void modfd(int epfd, int fd, int ev);

int main(int argc, char* argv[])
{
    if(argc <= 1)
    {
        printf("输入格式： %s port_number\n", basename(argv[0]));
        exit(-1);
    }

    int port = atoi(argv[1]);

    // 针对SIGPIPE
    addsig(SIGPIPE, SIG_IGN);

    // 线程池
    MyThreadpool<HTTPCONN>* mythreadpool = NULL;
    try
    {
        mythreadpool = new MyThreadpool<HTTPCONN>;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    // 所有的客户端信息
    HTTPCONN* users = new HTTPCONN[MAX_FD_SZ];

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    int reuse = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(listenfd, (struct sockaddr *)& addr, sizeof(addr));

    listen(listenfd, 5);

    epoll_event events[MAX_EVENTS_SZ];
    int epfd = epoll_create(5);

    addfd(epfd, listenfd, false);
    HTTPCONN::epfd_ = epfd;

    while(1)
    {
        int num = epoll_wait(epfd, events, MAX_EVENTS_SZ, -1);
        if((num < 0) && (errno != EINTR))
        {
            printf("epoll failure\n");
            break;
        }

        for(int i = 0; i < num; i++)
        {
            int sockfd = events[i].data.fd;
            // 有客户端连接
            if(sockfd == listenfd)
            {
                sockaddr_in client;
                socklen_t client_len = sizeof(client);

                int connfd = accept(sockfd, (struct sockaddr*)&client, &client_len);

                if(HTTPCONN::user_num_ >= MAX_FD_SZ)
                {
                    // 通知客户端，服务端连接队列已满
                    close(connfd);
                    continue;
                }

                users[connfd].init(connfd, client);
            }
            else if(events[i].events & (EPOLLHUP | EPOLLRDHUP | EPOLLERR))
            {
                // 异常关闭
                users[sockfd].close_conn();
            }
            else if(events[i].events & EPOLLIN)
            {
                if(users[sockfd].read())
                { // 一次性读完
                    mythreadpool->append_request(users + sockfd);
                }
                else
                {
                    users[sockfd].close_conn();
                }
            }
            else if(events[i].events & EPOLLOUT)
            {
                if(!users[sockfd].write())
                {
                    users[sockfd].close_conn();
                }
            }
        }
    }
    close(epfd);
    close(listenfd);
    delete[] users;
    delete mythreadpool;
}