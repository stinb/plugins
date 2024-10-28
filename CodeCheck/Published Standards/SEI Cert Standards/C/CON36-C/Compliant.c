#if __has_include(<threads.h>)

#include <stddef.h>
#include <threads.h>

struct node_t
{
    void *node;
    struct node_t *next;
};

struct node_t list;
static mtx_t lock;
static cnd_t condition;

void consume_list_element(void)
{
    if (thrd_success != mtx_lock(&lock))
    {
        /* Handle error */
    }

    while (list.next == NULL)
    {
        if (thrd_success != cnd_wait(&condition, &lock)) // UndCC_Valid
        {
            /* Handle error */
        }
    }

    /* Proceed when condition holds */

    if (thrd_success != mtx_unlock(&lock))
    {
        /* Handle error */
    }
}

#endif // __has_include(<threads.h>)
