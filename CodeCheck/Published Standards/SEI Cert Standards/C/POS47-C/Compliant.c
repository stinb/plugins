// SEI CERT C

#if __has_include(<pthread.h>)

#include <pthread.h>

volatile int a = 5;
volatile int b = 10;

/* Lock to enable threads to access a and b safely */
pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;

void *worker_thread(void *dummy)
{
    int c;
    int result;

    while (1)
    {
        if ((result = pthread_mutex_lock(&global_lock)) != 0)
        {
            /* handle error */
        }
        c = b;
        b = a;
        a = c;
        if ((result = pthread_mutex_unlock(&global_lock)) != 0)
        {
            /* handle error */
        }

        /* now we're safe to cancel, creating cancel point */
        pthread_testcancel();
    }
    return NULL;
}

#endif // __has_include(<pthread.h>)
