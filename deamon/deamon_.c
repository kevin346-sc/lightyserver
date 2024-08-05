#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define ERR_EXIT(m) \
do\
{\
    perror(m);\
    exit(EXIT_FAILURE);\
}\
while (0);\

void myhandler(int num)
{
    printf("the signal receive: %d\n", num);
    time_t tloc;
    time(&tloc);
    char buf[1024];
    memset(buf, 0, 1024);
    sprintf(buf, "%li\n", tloc);
    int fd = open("time.txt", O_RDWR | O_CREAT | O_APPEND, 0664);
    write(fd, buf, strlen(buf));
    close(fd);

}
// 新建子进程，退出父进程，子进程中新建会话，脱离控制终端，文件描述符重定向，设置掩码给予权限，更改工作目录
int main()
{
    pid_t pid = fork();
    if(pid > 0)
    {
        exit(0);
        //ERR_EXIT("fork");
    }

    setsid();

    int fd = open("/dev/null", O_RDWR);
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    /*
    int max_fd = sysconf(_SC_OPEN_MAX);
    for(int i = 0; i < max_fd; ++i)
    {
        close(i);
    }
    open("/dev/null", O_RDWR);
    dup(0);
    dup(0);
    */


    chdir("/home/webserver/");

    umask(0);

    // 注册信号
    struct sigaction act;
    act.sa_handler = myhandler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGALRM, &act, NULL);

    // 设置定时器
    struct itimerval it;
    it.it_interval.tv_sec = 2;
    it.it_interval.tv_usec = 0;
    it.it_value.tv_sec = 3;
    it.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &it, NULL);

    while(1)
    {

    }
    return 0;
}