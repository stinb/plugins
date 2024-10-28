#if __has_include(<pthread.h>)

#include <pthread.h>

void func(void *foo)
{
    /* Execution of thread */
}

int main(void)
{
    int result;
    pthread_t thread;

    if ((result = pthread_create(&thread, NULL, func, 0)) != 0)
    {
        /* Handle Error */
    }
    if ((result = pthread_cancel(thread)) != 0)
    {
        /* Handle Error */
    }

    /* Continue executing */

    return 0;
}

#endif // __has_include(<pthread.h>)
