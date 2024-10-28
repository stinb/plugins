#if __has_include(<pthread.h>)

#include <stdatomic.h>
#include <stddef.h>
#include <pthread.h>

pthread_mutex_t lock;
/* Atomic so multiple threads can increment safely */
atomic_int completed = ATOMIC_VAR_INIT(0);
enum
{
    max_threads = 5
};

int do_work(void *dummy)
{
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

    return 0;
}

int main(void)
{
    pthread_t threads[max_threads];

    if (0 != pthread_mutex_init(&lock, PTHREAD_MUTEX_DEFAULT))
    {
        /* Handle error */
    }
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

    pthread_mutex_destroy(&lock);
    return 0;
}

#endif // __has_include(<pthread.h>)
