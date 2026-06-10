#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>

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
        // child: redirect standard output to a file
        printf("child\n");
        // int wc = wait(NULL);
        // printf("hello, I am child (wc:%d) (pid:%d)\n",
        //        wc, (int)getpid());
    }
    else
    {
        // parent goes down this path (original process)
        int wc = wait(NULL);
        assert(wc >= 0);
        printf("hello, I am parent of %d (wc:%d) (pid:%d)\n",
               rc, wc, (int)getpid());
    }
    return 0;
}
