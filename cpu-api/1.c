#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int x = 100;
    printf("x before fork:%d\n", x);
    int rc = fork();
    if (rc < 0)
    {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        // child (new process)
        printf("hello, I am child (pid:%d)\n", (int)getpid());
        printf("x in child before change:%d\n", x);
        x = 50;
        printf("x in child after change:%d\n", x);
    }
    else
    {
        // parent goes down this path (original process)
        // int wc = wait(NULL);
        printf("x in parent before change:%d\n", x);
        x = 70;
        printf("x in parent after change:%d\n", x);
    }
    return 0;
}
