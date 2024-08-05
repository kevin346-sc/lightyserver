#include <unistd.h>
#include <stdio.h>

int main()
{
    int flag = fork();
    if(flag > 0)// 父进程
    {
        printf("parent, pid : %d\n", getpid());
        sleep(1);
        
    }
    else// 子进程
    {
        printf("child, pid:%d , ppid:%d\n", getpid(), getppid());
        // int ret = execl("/home/kevin/webserver/process/tesst", "tesst",(char *)NULL);
        // int ret = execlp("/bin/ls", "ls", "-l", NULL);

        char* env[] = {"test", NULL};
        int ret = execv("/home/kevin/webserver/process/test", env);

        if(ret == -1)
        {
            perror("execl");
            return -1;
        }
    }
    
    return 0;
}