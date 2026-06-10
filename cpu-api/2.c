#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int filedesc = open("2.txt", O_WRONLY | O_CREAT);
    printf("filedesc %d\n", filedesc);
    write(filedesc, "before fork\n", strlen("before fork\n"));
    int rc = fork();
    if (rc < 0)
    {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        // child: redirect standard output to a file
        write(filedesc, "child\n", strlen("child\n"));
    }
    else
    {
        // parent goes down this path (original process)
        int wc = wait(NULL);
        assert(wc >= 0);
        write(filedesc, "parent\n", strlen("parent\n"));
    }
    close(filedesc);
    return 0;
}
