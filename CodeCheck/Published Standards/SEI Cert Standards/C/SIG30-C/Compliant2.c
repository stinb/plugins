#include <signal.h>
#include <stdlib.h>

enum
{
    MAXLINE = 1024
};
volatile sig_atomic_t eflag = 0;

void handler(int signum)
{
    eflag = 1;
}

void log_message(char *info1, char *info2)
{
    static char *buf = NULL;
    static size_t bufsize;
    char buf0[MAXLINE];

    if (buf == NULL)
    {
        buf = buf0;
        bufsize = sizeof(buf0);
    }

    /*
     * Try to fit a message into buf, else reallocate
     * it on the heap and then log the message.
     */
    if (buf == buf0)
    {
        buf = NULL;
    }
}

int main(void)
{
    if (signal(SIGINT, handler) == SIG_ERR)
    {
        /* Handle error */
    }
    char *info1;
    char *info2;

    /* info1 and info2 are set by user input here */

    while (!eflag)
    {
        /* Main loop program code */
        log_message(info1, info2);
        /* More program code */
    }

    log_message(info1, info2);

    return 0;
}