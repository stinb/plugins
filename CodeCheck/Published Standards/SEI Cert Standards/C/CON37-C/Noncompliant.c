#if __has_include(<threads.h>)

#include <signal.h>
#include <stddef.h>
#include <threads.h>

#ifndef SIGUSR1
  #define SIGUSR1 0
#endif

volatile sig_atomic_t flag = 0;

void handler(int signum)
{
    flag = 1;
}

/* Runs until user sends SIGUSR1 */
int func(void *data)
{
    while (!flag)
    {
        /* ... */
    }
    return 0;
}

int main(void)
{
    signal(SIGUSR1, handler); // UNDCC_Violation(win,lin) invokes the signal() function from a multithreaded program
    thrd_t tid;

    if (thrd_success != thrd_create(&tid, func, NULL))
    {
        /* Handle error */
    }
    /* ... */
    return 0;
}

#endif // __has_include(<threads.h>)
