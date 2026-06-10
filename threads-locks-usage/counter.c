#define _GNU_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include "common.h"
#include "common_threads.h"

#define AMOUNT 1000000

typedef struct __counter_t
{
    int value;            // count
    pthread_mutex_t lock; // lock
} counter_t;

counter_t counter;

// init: init lock, init values
void init(counter_t *c)
{
    c->value = 0;
    pthread_mutex_init(&c->lock, NULL);
}

void increment(counter_t *c)
{
    pthread_mutex_lock(&c->lock);
    c->value++;
    pthread_mutex_unlock(&c->lock);
}

void decrement(counter_t *c)
{
    pthread_mutex_lock(&c->lock);
    c->value--;
    pthread_mutex_unlock(&c->lock);
}

// get: just return counter
int get(counter_t *c)
{
    pthread_mutex_lock(&c->lock);
    int rc = c->value;
    pthread_mutex_unlock(&c->lock);
    return rc;
}

void *mythread(void *arg)
{
    int amount = (int)arg;
    // on Linux
    // pid_t thread_id = gettid();

    // on MacOs
    /** int pthread_threadid_np(pthread_t thread, uint64_t *thread_id);
     * The pthread_threadid_np() function stores the system-wide
     * unique integral ID of thread in the location spedified by thread_id.
     * If thread is NULL, the ID of the current thread is provided. */
    uint64_t thread_id;
    pthread_threadid_np(NULL, &thread_id);
    for (int i = 0; i < amount; i++)
    {
        increment(&counter);
    }
    return NULL;
}

/** int gettimeofday(struct timeval *restrict tv,
 *struct timezone *_Nullable restrict tz);
 *
 *struct timeval {
 *   time_t      tv_sec;     // seconds
 *   suseconds_t tv_usec;    // microseconds
 *
 *ssize_t read(int fd, void buf[count], size_t count);
 */

// Simple routine to return absolute time (in microseconds).
long long Time_GetMicroSeconds()
{
    struct timeval t;
    int rc = gettimeofday(&t, NULL);
    assert(rc == 0);
    return (long long)(t.tv_sec) * 1e6 + t.tv_usec;
}

int main(int argc, char *argv[])
{
    init(&counter);
    printf("before: %d\n", get(&counter));

    pthread_t p1, p2, p3, p4;
    long long t = Time_GetMicroSeconds(); // microseconds
    Pthread_create(&p1, NULL, mythread, (void *)AMOUNT);
    Pthread_create(&p2, NULL, mythread, (void *)AMOUNT);
    Pthread_create(&p3, NULL, mythread, (void *)AMOUNT);
    Pthread_create(&p4, NULL, mythread, (void *)AMOUNT);

    // join waits for the threads to finish
    Pthread_join(p1, NULL);
    Pthread_join(p2, NULL);
    Pthread_join(p3, NULL);
    Pthread_join(p4, NULL);
    printf("after: %d\n", get(&counter));
    long long diff = Time_GetMicroSeconds() - t;
    printf("%lld microseconds for 4 threads, each incrementing %d\n", diff, AMOUNT);
    return 0;
}