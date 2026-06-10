#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "common_threads.h"

//
// Here, you have to write (almost) ALL the code. Oh no!
// How can you show that a thread does not starve
// when attempting to acquire this mutex you build?
// We want Threads to enter in batches
// No new threads to “cut in line” once a batch starts moving forward

typedef struct __ns_mutex_t
{
    int room1;   // number of threads in the waiting room one. Waiting area (threads arrive here)
    int room2;   // number of threads in the waiting room two. Next staging area (closer to critical section)
    sem_t mutex; // basic lock to protects counters
    sem_t t1;    // turnstile 1 between room 1 and 2
    sem_t t2;    // turnstile 2 between room 2 and critical section
} ns_mutex_t;

ns_mutex_t lock;
int counter = 0;

// Threads can enter Room 2 (t1 1)
// But cannot enter the critical section yet (t2 0)
void ns_mutex_init(ns_mutex_t *m)
{
    m->room1 = 0;
    m->room2 = 0;
    Sem_init(&m->mutex, 1); // initially open
    Sem_init(&m->t1, 1);    // initially open
    Sem_init(&m->t2, 0);    // initially closed
}

void ns_mutex_acquire(ns_mutex_t *m)
{
    // Enter Room 1 safely
    Sem_wait(&m->mutex);
    m->room1++;
    Sem_post(&m->mutex);

    // waiting on turnstile 1 to enter room2
    // Initially t1 = 1, so first thread passes immediately.
    Sem_wait(&m->t1);
    // we are in room 2 so update counters
    m->room2++;

    Sem_wait(&m->mutex);
    m->room1--;

    // Decide who to wake next
    // No one left in Room 1: This thread is the last of the batch
    if (m->room1 == 0)
    {
        Sem_post(&m->mutex); // release mutex for counters
        Sem_post(&m->t2);    // Open t2 so threads in room2 can go to Critical Section
    }
    else
    // Still threads in Room 1: Let next thread move from Room 1 → Room 2
    // 👉 Keep filling Room 2
    {
        Sem_post(&m->mutex);
        Sem_post(&m->t1);
    }
    // end of room 2, Wait at Turnstile 2
    // Only opens when Entire batch moved into Room 2
    Sem_wait(&m->t2);
    // Leaving Room 2
    m->room2--;
}

void ns_mutex_release(ns_mutex_t *m)
{
    // Room 2 is empty
    // All threads from the batch are done So:
    // Open t1 → allow next batch to start
    if (m->room2 == 0)
        Sem_post(&m->t1);
    // Still threads in Room 2 So:
    // Let next thread enter critical section to continue draining current batch
    else
        Sem_post(&m->t2);
}

void *worker(void *arg)
{
    ns_mutex_acquire(&lock);
    counter++;
    ns_mutex_release(&lock);
    return NULL;
}

int main(int argc, char *argv[])
{
    assert(argc == 2);
    int num_threads = atoi(argv[1]);
    pthread_t pt[num_threads];
    ns_mutex_init(&lock);
    printf("parent: begin\n");
    int i;
    for (i = 0; i < num_threads; i++)
        Pthread_create(&pt[i], NULL, worker, NULL);
    for (i = 0; i < num_threads; i++)
        Pthread_join(pt[i], NULL);
    printf("counter: %d\n", counter);
    printf("parent: end\n");
    return 0;
}
