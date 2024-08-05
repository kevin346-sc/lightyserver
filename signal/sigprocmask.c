#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main()
{
    sigset_t st; // 自定义阻塞信号集并添加到内核中
    sigemptyset(&st);

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

    sigprocmask(SIG_BLOCK, &st, NULL);
    // 获取系统当前的未决信号集
    sigset_t pendingset;
    sigemptyset(&pendingset);
    sigpending(&pendingset);

    int cnt = 0;
    while(1)
    {
        for(int i = 1; i <= 31; i++)
        {
            if(pendingset.__val[i] == 1)
                printf("1");
            else
                printf("0");
        }
        cnt++;
        printf("\n");
        sleep(1);
        // 撤销阻塞
        if(cnt == 15)
            sigprocmask(SIG_UNBLOCK, &st, NULL);
    }


    return 0;
}