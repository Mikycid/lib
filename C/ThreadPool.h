#ifndef DEF_THREAD_POOL
#define DEF_THREAD_POOL

// Implementation of a thread pool with multiple task types for better performance.
// Must include -lpthread flag in your compiler

/*
    Usage :
    Create the pool with createPool(n)
    You can add tasks into the main pool by using createTask(func, **args)
    For network operation there is an off thread created which can run those in an asyncio manner,
    you can use getLoop(*ThreadPool) and then use the loop like loop(func, **args) to have your network I/O operations running on 1 thread only
    Don't forget to free the pool with freePool(*ThreadPool) when you finished the operations.
*/


#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


typedef void (*thread_func)(void *args);


typedef struct Task {
    thread_func func;
    void *args;
    struct Task *next;
} Task;

typedef struct ThreadPool
{
    pthread_cond_t cond;
    pthread_cond_t working;
    pthread_mutex_t mutex;
    pthread_t *threads;
    size_t nThreads;
    Task *next;
    Task *last;
    size_t queueLength;
    char stop;
} ThreadPool;

static inline void *poolWorker(void *args);
static inline ThreadPool *createPool(size_t nThreads);
static inline void createTask(ThreadPool *pool, thread_func func, void *args);
static inline void joinPool(ThreadPool *pool);
static inline void freePool(ThreadPool *pool);
static inline void waitAll(ThreadPool *pool);




static inline void *poolWorker(void *args)
{
    ThreadPool *myContext = (ThreadPool *) args;
    while (1) {
        pthread_mutex_lock(&myContext->mutex);

        while(myContext->next == NULL && !myContext->stop)
            pthread_cond_wait(&myContext->cond, &myContext->mutex);
        
        if(myContext->stop && myContext->queueLength == 0)
            break;


        Task *task = myContext->next;
        if(task != NULL)
        {
            

            myContext->queueLength--;
            myContext->next = task->next;
            pthread_mutex_unlock(&myContext->mutex);

            thread_func func = task->func;
            void *args = task->args;



            func(args);
            pthread_cond_signal(&myContext->working);
            free(task);
        }
        else
        {
            pthread_mutex_unlock(&myContext->mutex);
        }

    }
    printf("thread destroy !\n");
    return NULL;
}

static inline ThreadPool *createPool(size_t nThreads)
{
    ThreadPool *pool = (ThreadPool *) malloc(sizeof(ThreadPool));

    pool->threads = (pthread_t *) malloc(sizeof(pthread_t) * nThreads);
    pool->nThreads = nThreads;
    pool->next = NULL;
    pool->last = NULL;
    pool->stop = 0;
    pool->queueLength = 0;

    pthread_cond_init(&pool->cond, NULL);
    pthread_cond_init(&pool->working, NULL);
    if(pthread_mutex_init(&pool->mutex, NULL) != 0)
    {
        printf("mutex initialization failed\n");
    }

    for(size_t i = 0; i < nThreads; i++)
    {
        pthread_create(&pool->threads[i], NULL, poolWorker, (void *)pool);
    }

    return pool;
}

static inline void createTask(ThreadPool *pool, thread_func func, void *args) {

    if(pool->stop)
    {
        printf("Could not create task %s : pool has been stopped", (char *) func);
        return;
    }
    Task *task = (Task *) malloc(sizeof(Task));
    task->func = func;
    task->args = args;
    task->next = NULL;
    pthread_mutex_lock(&pool->mutex);

    if(pool->next == NULL)
    {
        pool->next = task;
        pool->last = pool->next;
    }
    else
    {
        pool->last->next = task;
        pool->last = task;
    }


    pool->queueLength++;

    pthread_cond_broadcast(&pool->cond);
    pthread_mutex_unlock(&pool->mutex);


}

static inline void freePool(ThreadPool *pool)
{
    /*for(size_t i = 0; i < pool->nThreads; i++)
    {
        free(pool->threads[i]);
    }*/

    while(pool->next != NULL)
    {
        Task *current = pool->next;
        pool->next = current->next;
        free(current);
    }

    pthread_cond_destroy(&pool->cond);
    pthread_cond_destroy(&pool->working);
    pthread_mutex_destroy(&pool->mutex);
    free(pool->threads);
    free(pool);

}

static inline void waitAll(ThreadPool *pool)
{

    pthread_cond_broadcast(&pool->cond);
    while(pool->queueLength > 0)
    {
        printf("On attend les %lu derniers\n", pool->queueLength);
        pthread_cond_wait(&pool->working, &pool->mutex);
        pthread_cond_broadcast(&pool->cond);
    }
}

static inline void joinPool(ThreadPool *pool)
{
    // If wait, every task in the queue will be waited to end before the application may stop,
    // Else, it's the same as freePool

    pool->stop = 1;
    waitAll(pool);

    freePool(pool);
}

#endif