#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

/** int gettimeofday(struct timeval *restrict tv,
 *struct timezone *_Nullable restrict tz);
 *
 *struct timeval {
 *   time_t      tv_sec;     // seconds
 *   suseconds_t tv_usec;    // microseconds
 *
 *ssize_t read(int fd, void buf[count], size_t count);
 */
int main(int argc, char *argv[])
{
    // measure system call
    char buff[BUFSIZ];
    int N = 1e7; // N loops
    long diff;   // difference microseconds
    struct timeval tv0, tv1;
    gettimeofday(&tv0, NULL);
    printf("initial seconds %ld, microseconds %d\n",
           tv0.tv_sec, tv0.tv_usec);
    for (int i = 0; i < N; i++)
    {
        read(0, buff, 0); // read 0 bytes for N times
    }
    gettimeofday(&tv1, NULL);
    printf("final seconds %ld, microseconds %d\n",
           tv1.tv_sec, tv1.tv_usec);
    diff = (tv1.tv_sec - tv0.tv_sec) * 1e6;
    if (tv0.tv_usec > tv1.tv_usec)
    {
        diff += (tv0.tv_usec - tv1.tv_usec - 1e6);
    }
    else
    {
        diff += (tv1.tv_usec - tv0.tv_usec);
    }
    printf("difference microseconds %ld, duration microseconds/syscall %f\n",
           diff, (float)diff / N);

    // measure context switch
    // on MacOs there is no sched setaffinity()

    return 0;
}