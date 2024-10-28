#if __has_include(<pthread.h>)

#include <pthread.h>
#include <stdint.h>
#include <fcntl.h>

pthread_mutexattr_t attr;
pthread_mutex_t mutex;

void thread_foo(void *ptr)
{
    uint32_t num;
    int result;
    int sock;

    /* sock is a connected TCP socket */

    if ((result = recv(sock, (void *)&num, sizeof(uint32_t), O_NONBLOCK)) < 0)
    {
        /* Handle Error */
    }

    if ((result = pthread_mutex_lock(&mutex)) != 0)
    {
        /* Handle Error */
    }

    /* ... */

    if ((result = pthread_mutex_unlock(&mutex)) != 0)
    {
        /* Handle Error */
    }
}

#endif // __has_include(<pthread.h>)
