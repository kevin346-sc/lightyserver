// int kill(pid_t pid, int sig);
// int raise(int sig);
// void abort(void);
// unsigned int alarm(unsigned int seconds);
// int setitimer(int which, const struct itimerval *new_val,struct itimerval *old_value);

#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    pid_t pid = fork();
    if(pid > 0)
    {
        printf("parent, pid:%d\n", getpid());
        sleep(2);
        printf("kill now\n");
        int ret = kill(pid, SIGINT);
        
    }
    else if(pid == 0)
    {
        for(int i = 0; i < 5; i++)
        {
            printf("I am child\n");
            sleep(1);
        }
    }
    return 0;
}