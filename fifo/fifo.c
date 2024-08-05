#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int exsist = access("ff", F_OK);
    if(exsist == 0)
    {
        printf("exsist\n");
        exit(0);
    }
    int ret = mkfifo("ff", 0764);
    if(ret == -1)
    {
        perror("mkfifo");
        exit(-1);
    }

    return 0;
}

