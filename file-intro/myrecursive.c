#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

#define BUFFSIZE 256
// usage: ./myrecursive [dirname]

int list(char *dirname, int indent)
{
    int rc, curr;
    struct stat s;
    DIR *dp;
    struct dirent *d;
    char buff[BUFFSIZE];
    buff[0] = '\0'; // empty string
    // printf("dirname: %s\n", dirname);
    dp = opendir(dirname);
    assert(dp != NULL);

    while ((d = readdir(dp)) != NULL)
    {
        // don't print . and ..
        if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
        {
            continue;
        }
        curr = indent;
        while (curr > 0)
        {
            printf(" ");
            curr--;
        }
        printf("%s\n", d->d_name);
        strlcat(buff, dirname, BUFFSIZE);
        strlcat(buff, "/", BUFFSIZE);
        strlcat(buff, d->d_name, BUFFSIZE);
        // printf("relative path: %s\n", buff);
        rc = stat(buff, &s);
        assert(rc == 0);
        // printf("file type: %d\n", (int)s.st_mode);
        if ((s.st_mode & S_IFMT) == S_IFDIR)
        {
            list(buff, indent + 1);
        }
        buff[0] = '\0'; // empty string
    }
    closedir(dp);
    return 0;
}

int main(int argc, char *argv[])
{

    char *dirname = ".";
    if (argc > 2)
    {
        fprintf(stderr, "usage: ./myrecursive [dirname]\n");
        exit(1);
    }
    if (argc == 2)
    {
        dirname = argv[1]; // ./myrecursive dirname
    }
    list(dirname, 1);
    return 0;
}
