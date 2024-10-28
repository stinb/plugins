#if __has_include(<threads.h>)

#include <stddef.h>
#include <threads.h>

int thread_func(void *arg)
{
    /* Do work */
    thrd_detach(thrd_current());
    return 0;
}

int main(void)
{
    thrd_t t;

    if (thrd_success != thrd_create(&t, thread_func, NULL))
    {
        /* Handle error */
        return 0;
    }

    if (thrd_success != thrd_join(t, 0))  // UNDCC_Violation(win,lin) detaches a thread that is later joined
    {
        /* Handle error */
        return 0;
    }
    return 0;
}

#endif // __has_include(<threads.h>)
