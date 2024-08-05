#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void myhandler(int signum)
{
    printf("the signal type %d get\n", signum);
    printf("oh yeah\n");
}
// 每两秒，产生SIGALRM信号就结束当前进程
int main()
{
/*段错误
    // struct itimerval * newtimerval;
    // newtimerval->it_interval.tv_sec = 2; // 每隔2秒
    // newtimerval->it_interval.tv_usec = 0;
    // newtimerval->it_value.tv_sec = 3; // 3秒后开始
    // newtimerval->it_value.tv_usec = 0;
*/
    struct itimerval newtimerval;
    newtimerval.it_interval.tv_sec = 2; // 每隔2秒
    newtimerval.it_interval.tv_usec = 0;
    newtimerval.it_value.tv_sec = 3; // 3秒后开始
    newtimerval.it_value.tv_usec = 0;


    int ret = setitimer(ITIMER_REAL, &newtimerval, NULL);
    __sighandler_t st = signal(SIGALRM, myhandler);
    if(st == SIG_ERR)
    {
        perror("signal");
        exit(0);
    }
    
    printf("hhh\n");
    if(ret == -1)
    {
        perror("setitimer");
        exit(0);
    }
    getchar();
    return 0;
}