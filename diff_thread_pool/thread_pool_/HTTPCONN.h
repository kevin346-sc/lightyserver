#ifndef HTTPCONN_H
#define HTTPCONN_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
class HTTPCONN
{
public:
    HTTPCONN(/* args */){}
    ~HTTPCONN(){}

    static uint16_t epfd_ ;
    static uint16_t user_num_ ;
    static const int RD_BUF_SZ = 2048;
    static const int WR_BUF_SZ = 1024;

    void process(); // 处理客户端请求
    void init(int sockfd, const sockaddr_in& addr);
    void close_conn();
    bool read();
    bool write();

private:
    uint16_t sockfd_ {0};
    struct sockaddr_in addr_ {0};

    char rd_buf_[RD_BUF_SZ];
    int rd_idx_; // 读缓冲区已读数据的下标
    
};


#endif