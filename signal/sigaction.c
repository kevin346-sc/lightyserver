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


    
    // __sighandler_t st = signal(SIGALRM, myhandler);
    struct sigaction act;
    act.sa_handler = myhandler;
    sigset_t st;
    sigemptyset(&st);
    act.sa_mask = st;
    act.sa_flags = SA_RESTART;
    int  res = sigaction(SIGALRM, &act, NULL);
    if(res == -1)
    {
        perror("sigaction");
        exit(0);
    }

    int ret = setitimer(ITIMER_REAL, &newtimerval, NULL);
    printf("hhh\n");
    if(ret == -1)
    {
        perror("setitimer");
        exit(0);
    }
    getchar();
    // while(1);
    return 0;
}