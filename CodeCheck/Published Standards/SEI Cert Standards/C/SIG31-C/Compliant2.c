#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>

#ifdef __STDC_NO_ATOMICS__
#error "Atomics are not supported"
#elif ATOMIC_INT_LOCK_FREE == 0
#error "int is never lock-free"
#endif

atomic_int e_flag = ATOMIC_VAR_INIT(0);

void handler(int signum)
{
    e_flag = 1;  // UndCC_Valid
}

int main(void)
{
    enum
    {
        MAX_MSG_SIZE = 24
    };
    char err_msg[MAX_MSG_SIZE];
#if ATOMIC_INT_LOCK_FREE == 1
    if (!atomic_is_lock_free(&e_flag))
    {
        return EXIT_FAILURE;
    }
#endif
    if (signal(SIGINT, handler) == SIG_ERR)
    {
        return EXIT_FAILURE;
    }
    strcpy(err_msg, "No errors yet.");
    /* Main code loop */
    if (e_flag)
    {
        strcpy(err_msg, "SIGINT received.");
    }
    return EXIT_SUCCESS;
}
