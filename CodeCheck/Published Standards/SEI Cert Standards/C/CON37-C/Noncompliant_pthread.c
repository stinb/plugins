#if __has_include(<pthread.h>)

#include <signal.h>
#include <stddef.h>
#include <pthread.h>

volatile sig_atomic_t flag = 0;

void handler(int signum)
{
    flag = 1;
}

/* Runs until user sends SIGUSR1 */
void *func(void *data)
{
    while (!flag)
    {
        /* ... */
    }
    pthread_exit((void *)0);
}

int main(void)
{
    signal(SIGUSR1, handler); // UNDCC_Violation(mac,lin) invokes the signal() function from a multithreaded program
    pthread_t tid;

    if (0 != pthread_create(&tid, NULL, func, NULL))
    {
        /* Handle error */
    }
    /* ... */
    return 0;
}

#endif // __has_include(<pthread.h>)
