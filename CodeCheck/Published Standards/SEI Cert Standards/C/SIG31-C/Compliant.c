#include <signal.h>
#include <stdlib.h>
#include <string.h>

enum
{
    MAX_MSG_SIZE = 24
};
volatile sig_atomic_t e_flag = 0;

void handler(int signum)
{
    e_flag = 1;  // UndCC_Valid
}

int main(void)
{
    char *err_msg = (char *)malloc(MAX_MSG_SIZE);
    if (err_msg == NULL)
    {
        /* Handle error */
    }

    signal(SIGINT, handler);
    strcpy(err_msg, "No errors yet.");
    /* Main code loop */
    if (e_flag)
    {
        strcpy(err_msg, "SIGINT received.");
    }
    return 0;
}
