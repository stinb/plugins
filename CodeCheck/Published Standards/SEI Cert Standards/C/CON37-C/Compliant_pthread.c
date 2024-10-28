#if __has_include(<pthread.h>)

#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <pthread.h>

atomic_bool flag = ATOMIC_VAR_INIT(false);

void *func(void *data)
{
    while (!flag)
    {
        /* ... */
    }
    pthread_exit((void *)0);
}

int main(void)
{
    pthread_t tid;

    if (0 != pthread_create(&tid, NULL, func, NULL))
    {
        /* Handle error */
    }
    /* ... */
    /* Set flag when done */
    flag = true;

    return 0;
}

#endif // __has_include(<pthread.h>)
