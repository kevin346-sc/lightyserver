#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
    // 创建套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
        perror("socket");
        exit(-1);
    }

    // 连接
    struct sockaddr_in saddr;
    char * cp = "192.168.50.130";
    inet_pton(AF_INET, cp, &saddr.sin_addr.s_addr);
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    int ret = connect(fd, (struct sockaddr*)&saddr, sizeof(saddr));
    if(ret == -1)
    {
        perror("connect");
        exit(-1);
    }

    int i = 0;
    char buf[1024];
    while(1)
    {
        sprintf(buf, "client: %d\n", i++);
        write(fd, buf, strlen(buf));

        char buff[1024]={};
        ret = read(fd, buff, sizeof(buff));
        if(ret == -1)
        {
            perror("read");
            exit(-1);
        }
        else if(ret == 0)
        {
            printf("服务端断开连接\n");
            break;
        }
        else if(ret > 0)
            printf("client receive: %s\n", buf);
        sleep(1);
    }
    close(fd);
    return 0;
}