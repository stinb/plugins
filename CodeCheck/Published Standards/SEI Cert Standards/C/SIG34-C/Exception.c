#include <signal.h>

#ifdef SIGUSR1

void handler(int signum)
{
#if !defined(_WIN32)
    if (signal(signum, SIG_DFL) == SIG_ERR)  // UndCC_Valid - signal handler is reset to the system's default behavior
    {
        /* Handle error */
    }
#endif
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
