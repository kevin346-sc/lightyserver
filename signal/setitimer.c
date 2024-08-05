#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
    newtimerval.it_interval.tv_sec = 0; // 每隔2秒
    newtimerval.it_interval.tv_usec = 2;
    newtimerval.it_value.tv_sec = 0; // 3秒后开始
    newtimerval.it_value.tv_usec = 3;


    int ret = setitimer(ITIMER_VIRTUAL, &newtimerval, NULL);
    while(1)
    {
        printf("hhh\n");
        // sleep(1);
    }
    if(ret == -1)
    {
        perror("setitimer");
        exit(0);
    }
    getchar();
    return 0;
}