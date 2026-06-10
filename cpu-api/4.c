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
        // now exec "/bin/ls"...
        char *myargs[2];
        myargs[0] = strdup("/bin/ls"); // program: "/bin/ls" (list)
        myargs[1] = NULL;              // marks end of array

        /** exec() family of functions replaces the current process image
         * with a new process image.
         * v : vector 2nd arg is char *[] of [arg0, arg1, ..., NULL]
         *
         * l: list 2nd arg is char * of "arg0, arg1, ..., NULL"
         *
         * e: environment envp [arg0, arg1, ..., NULL] passed as the environment
         * of the new program. Functions that do not include 'e' in the suffix
         * take the environment for the new process image
         * from the external variable environ in the calling process
         *
         * p: path if the 1st arg (executable) does not contain a slash (/)
         * search in PATH */

        // Using execve: list of vectors (strings) v and provide environment (e)
        // char *envp[1];
        // envp[0] = NULL;
        // execve(myargs[0], myargs, envp); // runs ls

        // Using execl: 2nd arg is char * of "arg0, arg1, ..., NULL"
        // execl(myargs[0], "ls, NULL"); // runs ls

        // Using execlp: 2nd arg is char * of "arg0, arg1, ..., NULL"
        // and search in PATH
        // execlp(myargs[0], "ls, NULL"); // runs ls

        // Using execvp
        execvp(myargs[0], myargs); // runs ls
    }
    else
    {
        // parent goes down this path (original process)
        int wc = wait(NULL);
        assert(wc >= 0);
    }
    return 0;
}
