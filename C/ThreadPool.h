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



typedef void (*thread_func)(void *args);


typedef struct Task {
    thread_func func;
    void *args;
    struct Task *next;
    pthread_mutex_t mutex;
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

inline void *poolWorker(void *args);
inline ThreadPool *createPool(size_t nThreads);
inline void createTask(ThreadPool *pool, thread_func func, void *args);
inline void joinPool(ThreadPool *pool);
inline void freePool(ThreadPool *pool);
inline void waitAll(ThreadPool *pool);




inline void *poolWorker(void *args)
{
    ThreadPool *myContext = (ThreadPool *) args;
    pthread_mutex_t myMutex;
    pthread_mutex_init(&myMutex, NULL);
    while (1) {

        printf("on attend\n");
        while(myContext->next == NULL && !myContext->stop)
            pthread_cond_wait(&myContext->cond, &myMutex);
        printf("on attend plus\n");
        
        if(myContext->stop && myContext->queueLength == 0)
            break;

        int mt = pthread_mutex_lock(&myContext->mutex);
        if(mt != 0)
            printf("mutex didnt work\n");

        Task *task = myContext->next;
        if(task != NULL)
        {
            
            int inner_mt = pthread_mutex_trylock(&task->mutex);
            if(inner_mt != 0)
            {

                pthread_mutex_unlock(&myContext->mutex);
                continue;
            } 
            myContext->queueLength--;
            myContext->next = task->next;
            pthread_mutex_unlock(&myContext->mutex);

            thread_func func = task->func;
            void *args = task->args;



            func(args);
            pthread_cond_signal(&myContext->working);
            pthread_mutex_destroy(&task->mutex);
            free(task);
        }

    }
    pthread_mutex_destroy(&myMutex);
    printf("thread destroy !\n");
    return NULL;
}

inline ThreadPool *createPool(size_t nThreads)
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
        printf("tentative de créer\n");
        pthread_create(&pool->threads[i], NULL, poolWorker, (void *)pool);
        printf("thread %lu créé\n", i);
    }

    return pool;
}

inline void createTask(ThreadPool *pool, thread_func func, void *args) {

    if(pool->stop)
    {
        printf("Could not create task %s : pool has been stopped", (char *) func);
        return;
    }
    
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

inline void freePool(ThreadPool *pool)
{
    /*for(size_t i = 0; i < pool->nThreads; i++)
    {
        free(pool->threads[i]);
    }*/

    while(pool->next != NULL)
    {
        Task *current = pool->next;
        pool->next = current->next;
        pthread_mutex_destroy(&current->mutex);
        free(current);
    }

    pthread_cond_destroy(&pool->cond);
    pthread_cond_destroy(&pool->working);
    pthread_mutex_destroy(&pool->mutex);
    free(pool->threads);
    free(pool);

}

inline void waitAll(ThreadPool *pool)
{

    pthread_cond_broadcast(&pool->cond);
    while(pool->queueLength > 0)
    {
        printf("On attend les %lu derniers\n", pool->queueLength);
        pthread_cond_wait(&pool->working, &pool->mutex);
        printf("fini d'attendre\n");
        pthread_cond_broadcast(&pool->cond);
    }
}

inline void joinPool(ThreadPool *pool)
{
    // If wait, every task in the queue will be waited to end before the application may stop,
    // Else, it's the same as freePool

    pool->stop = 1;
    waitAll(pool);

    freePool(pool);
}

#endif