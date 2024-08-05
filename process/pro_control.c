#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>



int main(){
    // 孤儿进程——父进程在子进程之前退出
    // int ret = fork();
    // if(ret > 0)
    // {
    //     printf("parent, pid: %d, ppid: %d\n", getpid(), getppid());
    //     exit(0);
    // }
    // else
    // {
    //     printf("child, pid: %d, ppid: %d\n", getpid(), getppid());
    //     sleep(1);//保证让父进程先退出
    //     printf("orphan child, pid: %d, ppid: %d\n", getpid(), getppid());
    // }



    // 僵尸进程——子进程在父进程之前退出，但父进程不进行管理
    // int ret = fork();
    // if(ret > 0)
    // {
    //     printf("parent, pid: %d, ppid: %d\n", getpid(), getppid());
    //     sleep(2); // 保证让子进程先退出
    //     execlp("/bin/ps", "ps", "aux", NULL);
    //     exit(0);
    // }
    // else
    // {
    //     printf("child, pid: %d, ppid: %d\n", getpid(), getppid());
    //     exit(0);
    // }

    // wait函数解决僵尸进程
    // 创建五个子进程
    pid_t ret;
    for(int i = 0; i < 5; i++)
    {
        ret = fork();
        if(ret == 0)// 子进程则不再继续创建子进程
            break;
    }
    if(ret < 0)
    {
        perror("folk");
        return -1;
    }
    else if(ret > 0)
    {
        while(1)
        {
            printf("parent, pid: %d, ppid: %d\n", getpid(), getppid());
            pid_t res;
            int d;
            res = wait(&d); // 阻塞父进程
            // res = waitpid(-1, &d, WNOHANG); // 非阻塞
            if(res < 0) // no child
            {
                printf("no child left");
                return -1;
            }
            else if(res > 0) // 有子进程退出
            {
                if(WIFEXITED(d))
                    printf("child id: %d exit normally\n", res);
                if(WIFSIGNALED(d))
                    printf("child id: %d was kill by signal: %d\n", res, WTERMSIG(d));
            }
            else // 只有waitpid 才能返回0
                sleep(1);
        }
        exit(0);
    }
    else
    {
        while(1)
        {
            printf("child, pid: %d, ppid: %d\n", getpid(), getppid());
            sleep(1);
        }
        exit(0);
    }

    return 0;
}