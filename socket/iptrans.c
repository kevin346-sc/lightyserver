#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    char * src = "192.168.1.46";
    unsigned int num = 0;
    // char a[13]="";
    int ret = inet_pton(AF_INET, src, &num);
    unsigned char* p = (char*)&num;
    // int ret = inet_pton(AF_INET, src, a);
    // printf("%s\n", a);
    if(ret == -1)
    {
        perror("inet_pton");
        exit(0);
    }
    printf("%d, %x\n", num, num); // 小端字节序
    printf("%d %d %d %d\n",*p, *(p+1),*(p+2),*(p+3));

    char dst[16]="";
    const char *buf= inet_ntop(AF_INET, &num, dst, 16);
    printf("%s\n", buf);
    printf("%s\n", dst);
    return 0;
}
