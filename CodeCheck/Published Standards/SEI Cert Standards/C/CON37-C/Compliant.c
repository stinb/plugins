#if __has_include(<threads.h>)

#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <threads.h>

atomic_bool flag = ATOMIC_VAR_INIT(false);

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
    thrd_t tid;

    if (thrd_success != thrd_create(&tid, func, NULL))
    {
        /* Handle error */
    }
    /* ... */
    /* Set flag when done */
    flag = true;

    return 0;
}

#endif // __has_include(<threads.h>)
