#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>


int main()
{
    // 新建共享内存
    int shmid = shmget(100, 0, IPC_CREAT);
    // 关联当前进程
    void* ptr = shmat(shmid, NULL, 0);
    printf("%s", (char *)ptr);
    // 等待接受再删除
    printf("waiting....\n");
    getchar();
    // 解关联
    shmdt(ptr);
    // 删除共享内存
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}