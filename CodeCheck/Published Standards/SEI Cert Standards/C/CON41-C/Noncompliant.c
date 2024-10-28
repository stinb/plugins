#include <stdatomic.h>
#include <stdbool.h>
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
    struct data *new_head;
    bool success;

    /* ... Reorganize the data structure ... */

    success = atomic_compare_exchange_weak(ptr_to_head, &old_head, new_head); // UNDCC_Violation, atomic_compare_exchange_weak not wrapped in a loop
    if (!success)
    {
        cleanup_data_structure(new_head);
    }

    return success; /* Exit the thread */
}