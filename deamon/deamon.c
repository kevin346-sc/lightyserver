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

void work(int num)
{
    time_t tm = time(NULL);
    struct tm* loc = localtime(&tm);

    char* str = asctime(loc);
    int fd = open("time.txt", O_RDWR | O_CREAT | O_APPEND, 0664);
    write(fd, str, strlen(str));
    close(fd);
}

int main()
{
    // 创建子进程， 父进程退出
    pid_t pid = fork();
    if(pid > 0)
        exit(0);
    // 创建新会话
    setsid();

    // 设置掩码
    umask(0);

    // 更改工作目录到/home/webserver
    chdir("/home/webserver/");

    // 文件描述符
    int fd = open("/dev/null", O_RDWR);
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);

    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = work;
    sigemptyset(&act.sa_mask);
    sigaction(SIGALRM, &act, NULL);

    struct itimerval val;
    val.it_interval.tv_sec = 2;
    val.it_interval.tv_usec = 0;
    val.it_value.tv_sec = 3;
    val.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &val, NULL);
    while(1)
    {

    }
    return 0;
}