#include "HTTPCONN.h"

void setnonblock(int fd)
{
    int old = fcntl(fd, F_GETFL);
    int nw = old | O_NONBLOCK;
    fcntl(fd, F_SETFL, nw);
}

void addfd(int epfd, int fd, bool one_shot)
{
    struct epoll_event event;
    event.data.fd = fd;
    // event.events = EPOLLIN | EPOLLHUP;
    event.events = EPOLLIN | EPOLLHUP | EPOLLET;

    if(one_shot)
    {
        event.events | EPOLLONESHOT;
    }
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
    setnonblock(fd);
}

void delfd(int epfd, int fd)
{
    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, 0);
    close(fd);
}

void modfd(int epfd, int fd, int ev)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = ev | EPOLLONESHOT | EPOLLHUP;
    epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &event);
}

uint16_t HTTPCONN::user_num_ = 0;
uint16_t HTTPCONN::epfd_ = -1;

void HTTPCONN::init(int sockfd, const sockaddr_in& addr)
{
    this->sockfd_ = sockfd;
    this->addr_ = addr;

    int reuse = 1; 
    setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    
    addfd(epfd_, sockfd_, true);
    user_num_++;

}


void HTTPCONN::close_conn()
{
    if(sockfd_ != -1)
    {
        delfd(epfd_, sockfd_);
        sockfd_ = -1;
        user_num_--;
    }
}

bool HTTPCONN::read()
{
    printf("read...\n");
    if(rd_idx_ >= RD_BUF_SZ)
    {
        return false;
    }
    int bytes_read = 0;
    while(1)
    {
        bytes_read = recv(sockfd_, rd_buf_ + rd_idx_, RD_BUF_SZ - rd_idx_, 0);
        if(bytes_read == -1)
        {
            if(errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break;
            }
            return false;
        }
        else if(bytes_read == 0)
        {
            return false;
        }
        rd_idx_ += bytes_read;
    }
    printf("have read %s", rd_buf_);
    return true;
}

bool HTTPCONN::write()
{
    printf("write...\n");
    return true;
}

void HTTPCONN::process()
{
    // 解析HTTP请求并响应
    // HTTP_CODE http_ret = process_read();
    printf("process...\n");

}