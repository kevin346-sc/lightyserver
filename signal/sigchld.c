#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

void myhandler(int num)
{
    while(1)
    {
        pid_t pid = waitpid(-1,NULL,WNOHANG);   
        if(pid > 0)
            printf("child %d exit\n", pid);
        else
            break;
    }
}
int main()
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    sigprocmask(SIG_BLOCK, &set, NULL);

    int ret;
    for(int i = 0; i < 30; i++)
    {
        ret = fork();
        if(ret == 0)
            break;
    }
    if(ret == 0)
    {
        // child
        printf("I am child:%d\n", getpid());
    }
    else if(ret > 0)
    {
        // parent
        struct sigaction act;
        act.sa_handler = myhandler;
        sigset_t st;
        sigemptyset(&st);
        act.sa_mask = st;
        act.sa_flags = 0;
        int res = sigaction(SIGCHLD, &act, NULL);
        if(res == -1)
        {
            perror("sigaction");
            exit(0);
        }
        sigprocmask(SIG_UNBLOCK, &set, NULL);
        while(1)
        {
            printf("parent, pid : %d\n", getpid());
            sleep(2);
        }
    }
    else
    {
        perror("fork");
        exit(0);
    }
}