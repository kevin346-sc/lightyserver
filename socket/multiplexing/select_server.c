#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

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

    fd_set readfds, tmp;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    int maxfd = fd;
    while(1)
    {
        tmp = readfds;
        int ret = select(maxfd+1, &tmp, NULL, NULL, NULL);
        if(ret == -1)
        {
            perror("select");
            exit(-1);
        }
        else if(ret == 0)
            continue;
        else if(ret > 0)// 监听到有文件描述符缓冲区数据变化
        {
            // 文件描述符fd变化代表有新客户端连接
            if(FD_ISSET(fd, &tmp))
            {
                struct sockaddr_in addr_client;
                int size = sizeof(addr_client);
                int ffd = accept(fd, (struct sockaddr*)&addr_client, &size);

                FD_SET(ffd, &readfds);
                maxfd = ffd > maxfd? ffd : maxfd;
            }

            for(int i = fd + 1; i <= maxfd; i++)
            {
                // 缓冲区接受到数据
                if(FD_ISSET(i, &tmp))
                {
                    char buf[1024]={0};
                    int len = read(i, buf, sizeof(buf));
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
                        write(i, buf, sizeof(buf));
                    }
                }
            }
        }
    }
    close(fd);
    return 0;
}