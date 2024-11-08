// SEI CERT C

#if __has_include(<pthread.h>)

#include <pthread.h>

volatile int a = 5;
volatile int b = 10;

/* Lock to enable threads to access a and b safely */
pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;

void release_global_lock(void *dummy)
{
    int result;
    if ((result = pthread_mutex_unlock(&global_lock)) != 0)
    {
        /* handle error */
    }
}

void *worker_thread(void *dummy)
{
    int i;
    int c;
    int result;

    if ((result = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &i)) != 0) // UndCC_Violation(lin, mac)
    {
        /* handle error */
    }

    while (1)
    {
        if ((result = pthread_mutex_lock(&global_lock)) != 0)
        {
            /* handle error */
        }
        pthread_cleanup_push(release_global_lock, NULL);
        c = b;
        b = a;
        a = c;
        pthread_cleanup_pop(1);
    }
    return NULL;
}

#endif // __has_include(<pthread.h>)
