#if __has_include(<pthread.h>)

#include <stdatomic.h>
#include <stddef.h>
#include <pthread.h>

pthread_mutex_t lock;
/* Atomic so multiple threads can modify safely */
atomic_int completed = ATOMIC_VAR_INIT(0);
enum
{
    max_threads = 5
};

int do_work(void *arg)
{
    int *i = (int *)arg;

    if (*i == 0)
    { /* Creation thread */
        if (0 != pthread_mutex_init(&lock, PTHREAD_MUTEX_DEFAULT))
        {
            /* Handle error */
        }
        atomic_store(&completed, 1);
    }
    else if (*i < max_threads - 1)
    { /* Worker thread */
        if (0 != pthread_mutex_lock(&lock))
        {
            /* Handle error */
        }
        /* Access data protected by the lock */
        atomic_fetch_add(&completed, 1);
        if (0 != pthread_mutex_unlock(&lock))
        {
            /* Handle error */
        }
    }
    else
    { /* Destruction thread */
        pthread_mutex_destroy(&lock);   // UndCC_Violation(mac,lin)
    }
    return 0;
}

int main(void)
{
    pthread_t threads[max_threads];

    for (size_t i = 0; i < max_threads; i++)
    {
        if (0 != pthread_create(&threads[i], NULL, do_work, &i))
        {
            /* Handle error */
        }
    }
    for (size_t i = 0; i < max_threads; i++)
    {
        if (0 != pthread_join(threads[i], 0))
        {
            /* Handle error */
        }
    }
    return 0;
}

#endif // __has_include(<pthread.h>)