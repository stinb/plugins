#include <signal.h>
#include <stdlib.h>
#include <string.h>

enum
{
    MAX_MSG_SIZE = 24
};
char *err_msg;

void handler(int signum)
{
    strcpy(err_msg, "SIGINT encountered."); // UndCC_Violation
}

int main(void)
{
    signal(SIGINT, handler);

    err_msg = (char *)malloc(MAX_MSG_SIZE);
    if (err_msg == NULL)
    {
        /* Handle error */
    }
    strcpy(err_msg, "No errors yet.");
    /* Main code loop */
    return 0;
}