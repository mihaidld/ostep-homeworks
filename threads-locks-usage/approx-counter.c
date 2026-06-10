#define _GNU_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include "common.h"
#include "common_threads.h"

#define NUMCPUS 4
#define AMOUNT 1000000
#define THRESHOLD (1024)
// #define THRESHOLD (AMOUNT / 100)

typedef struct __counter_t
{
    int global;                     // global count
    pthread_mutex_t glock;          // global lock
    int local[NUMCPUS];             // per-CPU count
    pthread_mutex_t llock[NUMCPUS]; // ... and locks
    int threshold;                  // update freq
} counter_t;

counter_t counter;

// init: record threshold, init locks, init values
// of all local counts and global count
void init(counter_t *c, int threshold)
{
    c->threshold = threshold;
    c->global = 0;
    pthread_mutex_init(&c->glock, NULL);
    int i;
    for (i = 0; i < NUMCPUS; i++)
    {
        c->local[i] = 0;
        pthread_mutex_init(&c->llock[i], NULL);
    }
}

// update: usually, just grab local lock and update
// local amount; once it has risen ’threshold’,
// grab global lock and transfer local values to it
void update(counter_t *c, int threadID, int amt)
{
    int cpu = threadID % NUMCPUS;
    printf("threadID %d, cpu: %d\n", threadID, cpu);
    pthread_mutex_lock(&c->llock[cpu]);
    c->local[cpu] += amt;
    if (c->local[cpu] >= c->threshold)
    {
        // transfer to global (assumes amt>0)
        pthread_mutex_lock(&c->glock);
        c->global += c->local[cpu];
        pthread_mutex_unlock(&c->glock);
        c->local[cpu] = 0;
    }
    pthread_mutex_unlock(&c->llock[cpu]);
}

// get: just return global amount (approximate)
int get(counter_t *c)
{
    pthread_mutex_lock(&c->glock);
    int val = c->global;
    pthread_mutex_unlock(&c->glock);
    return val; // only approximate!
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
    update(&counter, (int)thread_id, amount);
    return NULL;
}

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
    init(&counter, THRESHOLD);
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
