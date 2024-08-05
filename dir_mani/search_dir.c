#define _DEFAULT_SOURCE
// #define _GNU_SOURCE
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int tongji(const char * path)
{
    DIR* dir = opendir(path);
    if(dir == NULL)
    {
        perror("opendir");
        exit(0);
    }
    int cnt = 0;
    struct dirent* dent;
    while((dent = readdir(dir)) != NULL)
    {
        char* dirname = dent->d_name;
        if(strcmp(dirname, ".") == 0|| strcmp(dirname, "..") == 0)
            continue;
        if(dent->d_type == DT_DIR)
        {
            char newpath[1024];
            sprintf(newpath, "%s/%s", path, dirname);
            cnt += tongji(newpath);
        }
        if(dent->d_type == DT_REG)
            cnt += 1;
    }
    closedir(dir);
    return cnt;
}
int main(int argc, char * argv[])
{
    if(argc < 2)
    {
        perror("too few args.");
        return -1;
    }

    int res = tongji(argv[1]);
    printf("普通文件个数为：%d\n", res);
    return 0;
}