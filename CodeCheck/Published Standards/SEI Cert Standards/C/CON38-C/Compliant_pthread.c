#if __has_include(<pthread.h>)

#include <stdio.h>
#include <pthread.h>

enum
{
    NTHREADS = 5
};

pthread_mutex_t mutex;
pthread_cond_t cond[NTHREADS];

void *run_step(void *t)
{
    static size_t current_step = 0;
    size_t my_step = *(size_t *)t;

    if (0 != pthread_mutex_lock(&mutex))
    {
        /* Handle error */
    }

    printf("Thread %zu has the lock\n", my_step);

    while (current_step != my_step)
    {
        printf("Thread %zu is sleeping...\n", my_step);

        if (0 != pthread_cond_wait(&cond[my_step], &mutex))
        {
            /* Handle error */
        }

        printf("Thread %zu woke up\n", my_step);
    }

    /* Do processing ... */
    printf("Thread %zu is processing...\n", my_step);

    current_step++;

    /* Signal next step thread */
    if ((my_step + 1) < NTHREADS)
    {
        if (0 != pthread_cond_signal(&cond[my_step + 1]))
        {
            /* Handle error */
        }
    }

    printf("Thread %zu is exiting...\n", my_step);

    if (0 != pthread_mutex_unlock(&mutex))
    {
        /* Handle error */
    }
    pthread_exit((void *)0);
}

int main(void)
{
    pthread_t threads[NTHREADS];
    size_t step[NTHREADS];

    if (0 != pthread_mutex_init(&mutex, 0))
    {
        /* Handle error */
    }

    for (size_t i = 0; i < NTHREADS; ++i)
    {
        if (0 != pthread_cond_init(&cond[i], NULL))
        {
            /* Handle error */
        }
    }

    /* Create threads */
    for (size_t i = 0; i < NTHREADS; ++i)
    {
        step[i] = i;
        if (0 != pthread_create(&threads[i], NULL, run_step,
                                &step[i]))
        {
            /* Handle error */
        }
    }

    /* Wait for all threads to complete */
    for (size_t i = NTHREADS; i != 0; --i)
    {
        if (0 != pthread_join(threads[i - 1], NULL))
        {
            /* Handle error */
        }
    }

    pthread_mutex_destroy(&mutex);

    for (size_t i = 0; i < NTHREADS; ++i)
    {
        pthread_cond_destroy(&cond[i]);
    }
    return 0;
}

#endif // __has_include(<pthread.h>)
