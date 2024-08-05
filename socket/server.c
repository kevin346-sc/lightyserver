#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    // 创建套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
        perror("socket");
        exit(0);
    }
    // 绑定
    // struct sockaddr addr;
    // addr.sa_family = AF_INET;
    // char *oriIP = "192.168.50.130";
    // // addr.sa_data = "192.168.50.130";
    // inet_pton(AF_INET, oriIP, &addr.sa_data);

    struct sockaddr_in saddr;
    // saddr.sin_addr = "192.168.50.130";
    char* oriIP = "192.168.50.130";
    inet_pton(AF_INET, oriIP, &saddr.sin_addr.s_addr);
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);


    // bind(fd, &addr, sizeof(addr));
    int ret = bind(fd, (struct sockaddr*)&saddr, sizeof(saddr));
    if(ret == -1)
    {
        perror("bind");
        exit(-1);
    }

    // 监听
    listen(fd, 8);
    if(ret == -1)
    {
        perror("listen");
        exit(-1);
    }

    // 连接
    struct sockaddr_in addr_client;
    socklen_t addr_client_t = sizeof(addr_client);
    int ffd = accept(fd, (struct sockaddr*)&addr_client, &addr_client_t);
    if(ffd == -1)
    {
        perror("accept");
        exit(-1);
    }
    // 客户端信息
    char ip_client[16];
    inet_ntop(AF_INET, &addr_client.sin_addr.s_addr, ip_client, sizeof(ip_client));
    int port_client = ntohs(addr_client.sin_port);
    printf("client ip : %s\t port : %d\n", ip_client, port_client);

    // 接收数据
    char buf[1024];
    ret = read(ffd, buf, sizeof(buf));
    if(ret == -1)
    {
        perror("read");
        exit(-1);
    }
    else if(ret == 0)
        printf("客户端断开连接");
    else if(ret > 0)
        printf("server receive: %s\n", buf);

    char *buff = "hello client, This is server";
    write(ffd, buff, strlen(buff));

    close(ffd);
    close(fd);

    return 0;
}