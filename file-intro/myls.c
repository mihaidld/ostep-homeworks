
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <dirent.h>

#define BUFFSIZE 256
// usage: ./myls [-l] [dirname]

int main(int argc, char *argv[])
{
    int rc;
    struct stat s;
    DIR *dp;
    struct dirent *d;

    char buff[BUFFSIZE];
    int islong = 0;
    char *dirname = ".";
    if (argc > 3)
    {
        fprintf(stderr, "usage: ./myls [-l] [dirname]\n");
        exit(1);
    }
    if (argc == 2)
    {
        if (strcmp(argv[1], "-l") == 0)
        {
            islong = 1; //./mysls -l
        }
        else
        {
            dirname = argv[1]; // ./myls dirname
        }
    }

    else if (argc == 3)
    {
        dirname = argv[2]; //./myls -l dirname
        islong = 1;
    }

    dp = opendir(dirname);
    assert(dp != NULL);

    while ((d = readdir(dp)) != NULL)
    {
        // don't print . and ..
        if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
        {
            continue;
        }
        printf("%s\n", d->d_name);
        // char * getcwd(char *buf, size_t size);
        // char * strcat( char * restrict destination, const char * restrict source );
        if (islong)
        {
            strlcat(buff, dirname, BUFFSIZE);
            strlcat(buff, "/", BUFFSIZE);
            strlcat(buff, d->d_name, BUFFSIZE);
            printf("relative path: %s\n", buff);
            rc = stat(buff, &s);
            assert(rc == 0);
            printf("file size: %d\n", (int)s.st_size);
            printf("number of blocks allocated: %d\n", (int)s.st_blocks);
            printf("reference (link) count: %d\n", (int)s.st_nlink);
            printf("inode number: %d\n", (int)s.st_ino);
            printf("owner: %d\n", s.st_uid);
            printf("group: %d\n", s.st_gid);
            printf("protection: %3o\n", s.st_mode & 0777);
            printf("time of last access: %s\n", ctime(&s.st_atime));
            buff[0] = '\0'; // empty string
        }
    }
    closedir(dp);
    return 0;
}
