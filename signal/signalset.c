#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main()
{
    sigset_t st;
    sigemptyset(&st);// 初始化set

    int res_2 = sigismember(&st, 2);
    int res_3 = sigismember(&st, 3);
    printf("%d\t", res_2);
    printf("%d\n", res_3);

    sigaddset(&st, 2);
    sigaddset(&st, 3);

    res_2 = sigismember(&st, 2);
    res_3 = sigismember(&st, 3);
    printf("%d\t", res_2);
    printf("%d\n", res_3);


    sigdelset(&st, 2);
    sigdelset(&st, 3);

    res_2 = sigismember(&st, 2);
    res_3 = sigismember(&st, 3);
    printf("%d\t", res_2);
    printf("%d\n", res_3);

    return 0;
}