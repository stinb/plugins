#if __has_include(<pthread.h>)

#include <pthread.h>
#include <stdlib.h>

/* Global key to the thread-specific storage */
pthread_key_t key;

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

    if (0 != pthread_setspecific(key, (void *)data))
    {
        /* Handle error */
    }
    return 0;
}

void print_data(void)
{
    /* Get this thread's global data from key */
    int *data = pthread_getspecific(key);

    if (data != NULL)
    {
        /* Print data */
    }
}

void *function(void *dummy)
{
    if (add_data() != 0)
    {
        /* Report error */
    }
    print_data();
    pthread_exit((void *)0);
}

void destructor(void *data)
{
    free(data);
}

int main(void)
{
    pthread_t thread_id[MAX_THREADS];

    /* Create the key before creating the threads */
    if (0 != pthread_key_create(&key, destructor)) // UndCC_Valid
    {
        /* Handle error */
    }

    /* Create threads that would store specific storage */
    for (size_t i = 0; i < MAX_THREADS; i++)
    {
        if (0 != pthread_create(&thread_id[i], NULL, function, NULL))
        {
            /* Handle error */
        }
    }

    for (size_t i = 0; i < MAX_THREADS; i++)
    {
        if (0 != pthread_join(thread_id[i], NULL))
        {
            /* Handle error */
        }
    }

    pthread_key_delete(key);
    return 0;
}

#endif // __has_include(<pthread.h>)
