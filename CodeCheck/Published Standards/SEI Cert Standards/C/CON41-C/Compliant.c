#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

struct data
{
    struct data *next;
    /* ... */
};

extern void cleanup_data_structure(struct data *head);

int reorganize_data_structure(void *thread_arg)
{
    struct data *_Atomic *ptr_to_head = thread_arg;
    struct data *old_head = atomic_load(ptr_to_head);
    struct data *new_head = NULL;
    struct data *saved_old_head;
    bool success;

    do
    {
        if (new_head != NULL)
        {
            cleanup_data_structure(new_head);
        }
        saved_old_head = old_head;

        /* ... Reorganize the data structure ... */

    } while (!(success = atomic_compare_exchange_weak(
                   ptr_to_head, &old_head, new_head)) &&
             old_head == saved_old_head);

    return success; /* Exit the thread */
}