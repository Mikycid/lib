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
    Don't forget to free the pool with destroyPool(*ThreadPool) when you finished the operations.
    Basically the queue size is of 20, you can define a queue size by using the INIT_QUEUE_SIZE macro if you know that there will be more elements from start
    Anyway if the queue happen to have more than 20 elements it will start reallocate more memory with currentSize * 2 every time
*/


#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct ThreadPool;

typedef void (*thread_func)(void *args);

typedef struct EventLoop {
    pthread_t thread;
    thread_func *queue;
    void **argsQueue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} EventLoop;

typedef struct Task {
    thread_func func;
    void *args;
    struct Task *next;
    pthread_mutex_t mutex;
} Task;

typedef struct ThreadPool
{
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    pthread_t *threads;
    size_t nThreads;
    Task *next;
    Task *last;
    size_t gap;
    size_t queueSize;
    thread_func *queue;
    void **argsQueue;
    size_t queueLength;
    EventLoop *loop;
} ThreadPool;

void *mainLoop(void *args)
{
    EventLoop *myContext = (EventLoop *) args;
    pthread_mutex_init(&myContext->mutex, NULL);
    pthread_cond_init(&myContext->cond, NULL);
    while(1)
    {
        pthread_cond_wait(&myContext->cond, &myContext->mutex);
    }
    return NULL;
}

void *poolWorker(void *args)
{
    ThreadPool *myContext = (ThreadPool *) args;
    pthread_mutex_t myMutex;
    pthread_mutex_init(&myMutex, NULL);
    while (1) {

        printf("on attend\n");
        while(myContext->next == NULL)
            pthread_cond_wait(&myContext->cond, &myMutex);
        printf("on attend plus\n");
        

        int mt = pthread_mutex_lock(&myContext->mutex);
        if(mt != 0)
            printf("mutex didnt work\n");

        Task *task = myContext->next;

        myContext->queueLength--;
        if(task != NULL)
        {
            int inner_mt = pthread_mutex_trylock(&task->mutex);
            if(inner_mt != 0)
            {

                pthread_mutex_unlock(&myContext->mutex);
                continue;
            } 

            pthread_mutex_unlock(&myContext->mutex);
            thread_func func = task->func;
            void *args = task->args;



            func(args);

            myContext->next = myContext->next->next;
            pthread_mutex_destroy(&task->mutex);
            free(task);
        }

    }
    pthread_mutex_destroy(&myMutex);
    return NULL;
}

ThreadPool *createPool(size_t nThreads)
{
    ThreadPool *pool = (ThreadPool *) malloc(sizeof(ThreadPool));

    pool->queueSize = 20;
    pool->argsQueue = (void **) malloc(sizeof(void *) * pool->queueSize);
    pool->queue = (thread_func *) malloc(sizeof(thread_func) * pool->queueSize);
    pool->threads = (pthread_t *) malloc(sizeof(pthread_t) * nThreads);
    pool->nThreads = nThreads;
    pool->next = NULL;
    pool->last = NULL;
    pool->queueLength = 0;

    pthread_cond_init(&pool->cond, NULL);
    if(pthread_mutex_init(&pool->mutex, NULL) != 0)
    {
        printf("mutex initialization failed\n");
    }

    for(size_t i = 0; i < nThreads; i++)
    {
        printf("tentative de créer\n");
        pthread_create(&pool->threads[i], NULL, poolWorker, (void *)pool);
        printf("thread %lu créé\n", i);
        //pthread_cond_wait(&pool->available, &pool->mutex);
    }
    pool->loop = (EventLoop *) malloc(sizeof(EventLoop));
    pthread_create(&pool->loop->thread, NULL, mainLoop, (void *)pool->loop);

    //pthread_mutex_unlock(&pool->mutex);
    return pool;
}

void createTask(ThreadPool *pool, thread_func func, void *args) {
    /*if(!pool->sleepyThreads)
    {
        printf("j'attend : il y a %d threads dispo\n", pool->sleepyThreads);
        pthread_cond_wait(&pool->available, &pool->mutex);
        printf("create task libéré !\n");
    }*/
    
    int mt = pthread_mutex_lock(&pool->mutex);

    if(mt != 0)
        printf("mutex inside createtask didnt work\n");

    if(pool->next == NULL)
    {
        pool->next = (Task *) malloc(sizeof(Task));
        pool->next->func = func;
        pool->next->args = args;
        pthread_mutex_init(&pool->next->mutex, NULL);
        pool->next->next = pool->last;
        pool->last = pool->next;
    }
    else
    {
        pool->last->next = (Task *) malloc(sizeof(Task));
        pool->last->next->func = func;
        pool->last->next->args = args;

        pthread_mutex_init(&pool->last->next->mutex, NULL);
        pool->last->next->next = NULL;
        pool->last = pool->last->next;
    }


    pool->queueLength++;

    pthread_cond_signal(&pool->cond);
    int rmt = pthread_mutex_unlock(&pool->mutex);

    if(rmt != 0)
        printf("mutex release inside createtask didnt work\n");


}

void freePool(ThreadPool *pool)
{
    /*for(size_t i = 0; i < pool->nThreads; i++)
    {
        free(pool->threads[i]);
    }*/

    pthread_cond_destroy(&pool->cond);
    pthread_mutex_destroy(&pool->mutex);
    free(pool->threads);
    free(pool->argsQueue);
    free(pool->queue);
    free(pool->loop);
    free(pool);

}

#endif