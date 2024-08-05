#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>

// 类型权限 硬链接数 用户名 组名 文件大小 修改时间 文件名
//-rw-rw-r-- 1 kevin kevin 0 5月   1 16:07 ls-l.c

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        perror("too few arguments");
        return -1;
    }
    struct stat st;
    int flag = stat(argv[1], &st);
    if(flag == -1)
    {
        perror("stat");
        return -1;
    }

    char filetype;
    switch(st.st_mode & __S_IFMT)
    {
        case __S_IFLNK:
            filetype = 'l';
            break;
        case __S_IFSOCK:
            filetype = 's';
            break;
        case __S_IFREG:
            filetype = '-';
            break;
        case __S_IFDIR:
            filetype = 'd';
            break;
        case __S_IFBLK:
            filetype = 'b';
            break;
        case __S_IFCHR:
            filetype = 'c';
            break;
        case __S_IFIFO:
            filetype = 'f';
            break;
        default:
            filetype = '?';
            break;
    }
    char quanxian[10];
    memset(quanxian, '-', sizeof(quanxian));
    // user
    if(st.st_mode & S_IRUSR)
        quanxian[1] = 'R';
    if(st.st_mode & S_IWUSR)
        quanxian[2] = 'W';
    if(st.st_mode & S_IXUSR)
        quanxian[3] = 'X';
    
    // group
    if(st.st_mode & S_IRGRP)
        quanxian[4] = 'R';
    if(st.st_mode & S_IWGRP)
        quanxian[5] = 'W';
    if(st.st_mode & S_IXGRP)
        quanxian[6] = 'X';
    
    // other
    if(st.st_mode & S_IROTH)
        quanxian[7] = 'R';
    if(st.st_mode & S_IWOTH)
        quanxian[8] = 'W';
    if(st.st_mode & S_IXOTH)
        quanxian[9] = 'X';
    // printf("%c%s\n", filetype,quanxian);

    // 硬链接数
    int linknum = st.st_nlink;

    // 组名 用户名
    char* u = getpwuid(st.st_uid)->pw_name;
    char* g = getgrgid(st.st_gid)->gr_name;
    
    // 文件大小
    long int fsize = st.st_size;

    // 文件名
    char* file_name = argv[1];

    // 修改时间
    char* t = ctime(&st.st_mtime); // 这个字符串包含最后的换行符
    // printf("%s\n", t);
    char tt[100];
    // strncpy(tt, t, sizeof(t) - 1); // Tue May
    strncpy(tt, t, strlen(t) - 1); // Tue May  2 10:15:54 2023
    // printf("%s\n", tt);

    // 最终输出结果
    char disp[1024] = {0};

    sprintf(disp, "%c%s %d %s %s %ld %s %s\n", filetype, quanxian, linknum, u, g, fsize, tt, file_name);
    // --RW-RW-R--Tue May  2 10:23:42 2023 1 kevin kevin 2590 Tue May  2 10:23:42 2023 ls-l.c
    // -rw-rw-r-- 1 kevin kevin  2668 5月   2 10:26 ls-l.c


    printf("%s", disp);
    return 0;
}