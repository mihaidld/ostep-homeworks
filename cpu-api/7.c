#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    int rc = fork();
    if (rc < 0)
    {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        // child: close standard output then print
        printf("child before\n");
        close(STDOUT_FILENO);
        int res = printf("child after\n");
        fprintf(stderr, "%d\n", res);
    }
    else
    {
        // parent goes down this path (original process)
        int wc = wait(NULL);
        assert(wc >= 0);
        printf("parent\n");
    }
    return 0;
}
