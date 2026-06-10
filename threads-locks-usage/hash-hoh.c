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
#define BUCKETS (101)

// basic node structure
typedef struct __node_t
{
    int key;
    pthread_mutex_t lock;
    struct __node_t *next;

} node_t;

// basic list structure (one used per list)
typedef struct __list_t
{
    node_t *head;
} list_t;

typedef struct __hash_t
{
    list_t lists[BUCKETS];
} hash_t;

hash_t h;

void List_Init(list_t *L)
{
    L->head = NULL;
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
    pthread_mutex_init(&new->lock, NULL);
    new->next = L->head;
    L->head = new;
    return 0; // success
}

int List_Lookup(list_t *L, int key)
{
    int rv = -1;
    node_t *curr = L->head;
    node_t *prev = NULL;

    // lock 1st node
    if (curr)
    {
        pthread_mutex_lock(&curr->lock);
    }
    while (curr)
    {
        if (curr->key == key)
        {
            rv = 0;
            pthread_mutex_unlock(&curr->lock);
            break;
        }
        prev = curr;
        curr = curr->next;
        // lock next node if there is one
        if (curr)
        {
            pthread_mutex_lock(&curr->lock);
        }
        // unlock previous node
        pthread_mutex_unlock(&prev->lock);
    }

    return rv; // now both success and failure
}

void Hash_Init(hash_t *H)
{
    int i;
    for (i = 0; i < BUCKETS; i++)
        List_Init(&H->lists[i]);
}

int Hash_Insert(hash_t *H, int key)
{
    return List_Insert(&H->lists[key % BUCKETS], key);
}
int Hash_Lookup(hash_t *H, int key)
{
    return List_Lookup(&H->lists[key % BUCKETS], key);
}

void *mythread(void *arg)
{
    int amount = (int)arg;
    for (int i = 0; i < amount; i++)
    {
        Hash_Insert(&h, i);
    }
    int rc = Hash_Lookup(&h, AMOUNT);
    printf("return code of lookup %d in hash table of 1's %d\n", AMOUNT, rc);
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

    Hash_Init(&h);

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
    printf("%lld microseconds for 4 threads, each adding %d numbers in [0, %d), at the end checking for %d inside hash table\n", diff, AMOUNT, AMOUNT, AMOUNT);
    return 0;
}
