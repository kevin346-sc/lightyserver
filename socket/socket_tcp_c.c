#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    // 创建套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
        perror("socket");
        exit(-1);
    }

    // 与服务端连接
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    char * cp = "192.168.50.130";
    inet_pton(AF_INET, cp, &addr.sin_addr.s_addr);
    int ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));

    // 发送数据
    char * buf = "hi, this message is from client, hope for your reply";
    write(fd, buf, strlen(buf)+1);

    // 接受数据
    char buff[1024]={0};
    ret = read(fd, buff, sizeof(buff));
    if(ret == 0)
    {
        printf("lost connection from server\n");
        exit(-1);
    }
    else if(ret == -1)
    {
        perror("read");
        exit(-1);
    }
    else if(ret > 0)
    {
        printf("I receive : %s\n", buff);
    }
    return 0;

}