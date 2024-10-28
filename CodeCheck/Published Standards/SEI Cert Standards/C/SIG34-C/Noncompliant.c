#include <signal.h>

#ifdef SIGUSR1

void handler(int signum)
{
    if (signal(signum, handler) == SIG_ERR) // UndCC_Violation(lin,mac)
    {
        /* Handle error */
    }
    /* Handle signal */
}

void func(void)
{
    if (signal(SIGUSR1, handler) == SIG_ERR)
    {
        /* Handle error */
    }
}

#endif // SIGUSR1
