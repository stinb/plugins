#if __has_include(<pthread.h>)

#include <stddef.h>
#include <pthread.h>

void *thread_func(void *arg)
{
    /* Do work */
    pthread_exit((void *)0);
}

int main(void)
{
    pthread_t t;

    if (0 != pthread_create(&t, NULL, thread_func, NULL))
    {
        /* Handle error */
        return 0;
    }

    if (0 != pthread_join(t, 0))
    {
        /* Handle error */
        return 0;
    }
    return 0;
}

#endif // __has_include(<pthread.h>)
