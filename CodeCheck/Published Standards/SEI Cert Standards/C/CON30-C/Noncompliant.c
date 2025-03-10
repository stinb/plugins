#if __has_include(<threads.h>)

#include <threads.h>
#include <stdlib.h>

/* Global key to the thread-specific storage */
tss_t key;
enum
{
    MAX_THREADS = 3
};

int *get_data(void)
{
    int *arr = (int *)malloc(2 * sizeof(int));
    if (arr == NULL)
    {
        return arr; /* Report error  */
    }
    arr[0] = 10;
    arr[1] = 42;
    return arr;
}

int add_data(void)
{
    int *data = get_data();
    if (data == NULL)
    {
        return -1; /* Report error */
    }

    if (thrd_success != tss_set(key, (void *)data))
    {
        /* Handle error */
    }
    return 0;
}

void print_data(void)
{
    /* Get this thread's global data from key */
    int *data = tss_get(key);

    if (data != NULL)
    {
        /* Print data */
    }
}

int function(void *dummy)
{
    if (add_data() != 0)
    {
        return -1; /* Report error */
    }
    print_data();
    return 0;
}

int main(void)
{
    thrd_t thread_id[MAX_THREADS];

    /* Create the key before creating the threads */
    if (thrd_success != tss_create(&key, NULL)) // UNDCC_Violation(win,lin)
    {
        /* Handle error */
    }

    /* Create threads that would store specific storage */
    for (size_t i = 0; i < MAX_THREADS; i++)
    {
        if (thrd_success != thrd_create(&thread_id[i], function, NULL))
        {
            /* Handle error */
        }
    }

    for (size_t i = 0; i < MAX_THREADS; i++)
    {
        if (thrd_success != thrd_join(thread_id[i], NULL))
        {
            /* Handle error */
        }
    }

    tss_delete(key);
    return 0;
}

#endif // __has_include(<threads.h>)
