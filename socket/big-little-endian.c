#include <arpa/inet.h>
#include <stdio.h>

int main()
{
    // 端口
    unsigned short port = 0x0102;
    printf("origin: %x\n", port);
    unsigned short port_after = htons(port);
    printf("after: %x\n", port_after);

    // ip地址
    char buf[4] = {192, 168, 1, 46};
    int num = *(int *)buf;
    printf("origin ip addr: %d\n", num);
    int num_after = htonl(num);
    printf("after: %d\n", num_after);
    unsigned char* p = (char*)&num_after;
    printf("%d %d %d %d\n",*p, *(p+1),*(p+2),*(p+3));

    return 0;
}