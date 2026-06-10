#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>

#define WRITE_END 1
#define READ_END 0

/** use pipe sys call and fork 2 child processes to execute
 * grep -o include 7.c | wc -l
 */
int main(int argc, char *argv[])
{
    int fd[2];   // file descriptors for pipe
    pid_t rc[2]; // process identifiers of 2 children
    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    rc[0] = fork(); // fork child1
    if (rc[0] < 0)
    {
        // fork failed; exit
        fprintf(stderr, "fork child 1 failed\n");
        exit(1);
    }
    else if (rc[0] == 0)
    {
        // child1: redirect standard output to a pipe write
        // child 1 writes to pipe
        /*dup2(int oldfd, int newfd) system call
        the file descriptor newfd is adjusted so that it now
        refers to the same open file description as oldfd */
        dup2(fd[WRITE_END], STDOUT_FILENO);
        close(fd[READ_END]);
        close(fd[WRITE_END]);

        // now exec "grep -o include 7.c"...
        execlp("grep", "grep", "-o", "include", "7.c", (char *)NULL);
        fprintf(stderr, "Failed to execute '%s'\n", "grep");
        exit(1);
    }
    else
    {
        // in parent
        rc[1] = fork(); // fork child 2
        if (rc[1] < 0)
        {
            // fork failed; exit
            fprintf(stderr, "fork child 2 failed\n");
            exit(1);
        }
        else if (rc[1] == 0)
        {
            // child2: redirect standard input from pipe read
            // child2 reads from pipe
            dup2(fd[READ_END], STDIN_FILENO);
            close(fd[WRITE_END]);
            close(fd[READ_END]);

            // now exec "wc -l"...
            execlp("wc", "wc", "-l", (char *)NULL);
            exit(1);
        }
        else
        {
            // parent goes down this path (original process)
            int status;
            close(fd[READ_END]);
            close(fd[WRITE_END]);
            waitpid(rc[0], &status, 0);
            printf("hello, I am parent of child1 %d and child2 %d (pid:%d)\n",
                   rc[0], rc[1], (int)getpid());
        }
        return 0;
    }
}