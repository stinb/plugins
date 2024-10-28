#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

enum
{
    MAXLINE = 1024
};
char *info = NULL;

void log_message(void)
{
    fputs(info, stderr);
}

void handler(int signum)
{
    log_message();
    free(info);
    info = NULL;
}

int main(void)
{
    if (signal(SIGINT, handler) == SIG_ERR) // UndCC_Violation
    {
        /* Handle error */
    }
    info = (char *)malloc(MAXLINE);
    if (info == NULL)
    {
        /* Handle Error */
    }

    while (1)
    {
        /* Main loop program code */

        log_message();

        /* More program code */
    }
    return 0;
}