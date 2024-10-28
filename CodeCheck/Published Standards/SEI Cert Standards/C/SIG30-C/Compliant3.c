#include <signal.h>
#include <stdlib.h>

void term_handler(int signum)
{
    /* SIGTERM handler */
}

void int_handler(int signum)
{
    /* SIGINT handler */
    /* Pass control to the SIGTERM handler */
    term_handler(SIGTERM);
}

int main(void)
{
    if (signal(SIGTERM, term_handler) == SIG_ERR)
    {
        /* Handle error */
    }
    if (signal(SIGINT, int_handler) == SIG_ERR)
    {
        /* Handle error */
    }

    /* Program code */
    if (raise(SIGINT) != 0)
    {
        /* Handle error */
    }
    /* More code */

    return EXIT_SUCCESS;
}