#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <errno.h>
#include <pthread.h>
#define LITHREAD 128

/// @bug 问题在于只创建存储在栈中的局部变量，而子线程中不共享栈空间；
//         而使用strcut Client_Info* client_info = malloc(...) 在堆中申请内存，需要在子线程释放内存，且可能会申请过多子线程导致内存溢出
// 解决： 在堆中申请定长数组空间
///

/// @bug 如果是struct sockaddr_in* addr_client， 为什么会发生段错误
struct Client_Info{
    struct sockaddr_in addr_client;
    pthread_t tid; // 线程号
    int fd; // 读写数据用的文件描述符
};


struct Client_Info client_infos[LITHREAD];

void* myroutine(void* arg)
{
    // 打印客户端信息
    struct Client_Info* client_info = (struct Client_Info*)arg;
    char buf[1024] = {0};
    
    inet_ntop(AF_INET, &client_info->addr_client.sin_addr.s_addr, buf, sizeof buf);
    printf("connect client ip: %s, port: %d\n", buf, ntohs(client_info->addr_client.sin_port));

    char buff[1024] = {0};
    int ffd = client_info->fd;
    while(1)
    {
        int ret = read(ffd, buff, sizeof(buff));
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
}
int main()
{
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
    int ret = bind(fd, (struct sockaddr*)&addr, sizeof addr);
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
    // 初始化定长数组
    for(int i = 0; i < LITHREAD; i++)
    {
        memset(&client_infos[i], -1, sizeof(client_infos[i]));
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

        /* 
        int i = 0;
        struct Client_Info client_info;
        client_info.addr_client = &addr_client;
        client_info.cnt = i++;
        client_info.fd = &ffd;
        pthread_create(&client_info.fd, NULL, myroutine, &client_info);// 传入客户端信息，ip + 端口号
        pthread_detach(client_info.fd);*/

        struct Client_Info* client_info;
        // 遍历定长数组，判断是否有空余位置，找到第一个空位
        for(int i = 0; i < LITHREAD; i++)
        {
            if(client_infos[i].fd == -1)
            {
                client_info = &client_infos[i];
                break;
            }
            if(i == LITHREAD - 1)
            {
                sleep(1);
                i--;
            }
        }
        int i = 0;
        // client_info->addr_client = &addr_client;
        // 结构体不能赋值运算，需要memcpy
        memcpy(&(client_info->addr_client), &addr_client, sizeof(addr_client));
        client_info->fd = ffd;
        pthread_create(&client_info->tid, NULL, myroutine, client_info);// 传入客户端信息，ip + 端口号
        pthread_detach(client_info->tid);

    }

    close(fd);

    return 0;
}