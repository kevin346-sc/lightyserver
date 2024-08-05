#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main()
{
    // 创建socket套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
        perror("socket");
        exit(-1);
    }

    // 套接字绑定ip，端口
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    const char* cp = "192.168.50.130";
    inet_pton(AF_INET, cp, &addr.sin_addr.s_addr);
    int ret = bind(fd, (struct sockaddr *)&addr, sizeof addr);
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

    // 连接
    struct sockaddr_in addr_client; // 储存客户端的信息
    int len = sizeof(addr_client);
    int ffd = accept(fd, (struct sockaddr*)&addr_client, &len);
    if(ffd == -1)
    {
        perror("accept");
        exit(-1);
    }
    // 打印客户端信息
    char buf[16] = {0};
    inet_ntop(AF_INET, &addr_client.sin_addr.s_addr, buf, sizeof(buf)); // ip
    printf("client ip: %s, port: %d\n", buf, ntohs(addr_client.sin_port));

    // 接受数据
    char buff[1024] = {0};
    ret = read(ffd, buf, sizeof(buff));
    if(ret > 0)
    {
        printf("server receive: %s\n", buff);
    }
    else if(ret == -1)
    {
        perror("read");
        exit(-1);
    }
    else if(ret == 0)
    {
        printf("lost connection..");
        exit(-1);
    }

    // 发送数据
    sprintf(buff, "hi, this is a message from server");
    write(ffd, buf, sizeof(buff));

    close(ffd);
    close(fd);

    return 0;
}