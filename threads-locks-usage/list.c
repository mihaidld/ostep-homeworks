#define _GNU_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include "common.h"
#include "common_threads.h"

#define AMOUNT 1000000

// basic node structure
typedef struct __node_t
{
    int key;
    struct __node_t *next;

} node_t;

// basic list structure (one used per list)
typedef struct __list_t
{
    node_t *head;
    pthread_mutex_t lock;

} list_t;

list_t l;

void List_Init(list_t *L)
{
    L->head = NULL;
    pthread_mutex_init(&L->lock, NULL);
}

int List_Insert(list_t *L, int key)
{
    // synchronization not needed
    node_t *new = malloc(sizeof(node_t));
    if (new == NULL)
    {
        perror("malloc");
        return -1;
    }
    new->key = key;
    // just lock critical section
    pthread_mutex_lock(&L->lock);
    new->next = L->head;
    L->head = new;
    pthread_mutex_unlock(&L->lock);
    return 0; // success
}

int List_Lookup(list_t *L, int key)
{
    int rv = -1;
    pthread_mutex_lock(&L->lock);
    node_t *curr = L->head;
    while (curr)
    {
        if (curr->key == key)
        {
            rv = 0;
            break;
        }
        curr = curr->next;
    }
    pthread_mutex_unlock(&L->lock);
    return rv; // now both success and failure
}

void *mythread(void *arg)
{
    int amount = (int)arg;
    for (int i = 0; i < amount; i++)
    {
        List_Insert(&l, 1);
    }
    int rc = List_Lookup(&l, 2);
    printf("return code of lookup 2 in list of 1's %d\n", rc);
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

    List_Init(&l);

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
    long long diff = Time_GetMicroSeconds() - t;
    printf("%lld microseconds for 4 threads, each adding %d 1's, at the end checking for 2 inside list\n", diff, AMOUNT);
    return 0;
}
