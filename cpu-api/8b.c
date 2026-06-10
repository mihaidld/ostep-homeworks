#include <errno.h>
#include <stdio.h>
#include <stdlib.h>   // exit
#include <string.h>   // strlen
#include <sys/wait.h> // waitpid
#include <unistd.h>   // fork, pipe, close, write, dup2

#define firstcmd "ls"
#define frsarg "-l"
#define scmd "wc"
#define secarg "-l"
#define WRITE_END 1
#define READ_END 0

/** https://stackoverflow.com/questions/33884291/pipes-dup2-and-exec */

int main()
{
    pid_t pid;
    int fd[2];

    pipe(fd);
    pid = fork();

    if (pid == 0)
    {
        dup2(fd[WRITE_END], STDOUT_FILENO);
        close(fd[READ_END]);
        close(fd[WRITE_END]);
        execlp(firstcmd, firstcmd, frsarg, (char *)NULL);
        fprintf(stderr, "Failed to execute '%s'\n", firstcmd);
        exit(1);
    }
    else
    {
        pid = fork();

        if (pid == 0)
        {
            dup2(fd[READ_END], STDIN_FILENO);
            close(fd[WRITE_END]);
            close(fd[READ_END]);
            execlp(scmd, scmd, secarg, (char *)NULL);
            fprintf(stderr, "Failed to execute '%s'\n", scmd);
            exit(1);
        }
        else
        {
            int status;
            close(fd[READ_END]);
            close(fd[WRITE_END]);
            waitpid(pid, &status, 0);
        }
    }
}