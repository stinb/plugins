#if __has_include(<pthread.h>)

#include <stddef.h>
#include <pthread.h>

struct node_t
{
    void *node;
    struct node_t *next;
};

struct node_t list;
static pthread_mutex_t lock;
static pthread_cond_t condition;

void consume_list_element(void)
{
    if (0 != pthread_mutex_lock(&lock))
    {
        /* Handle error */
    }

    if (list.next == NULL)
    {
        if (0 != pthread_cond_wait(&condition, &lock)) // UndCC_Violation(lin,mac) - pthread_cond_wait not wrapped inside a loop
        {
            /* Handle error */
        }
    }

    /* Proceed when condition holds */

    if (0 != pthread_mutex_unlock(&lock))
    {
        /* Handle error */
    }
}

#endif // __has_include(<pthread.h>)
