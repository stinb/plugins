#if defined(__linux__) || defined(_WIN32)

#include <threads.h>
#include <stdint.h>

mtx_t Ra;                         /* UndCC_Valid */
_Thread_local mtx_t thread_mutex; /* UndCC_Violation(lin, win), thread storage duration */

int32_t t1(void *ptr) /* Thread entry */
{
    mtx_lock(&Ra);
    mtx_lock((mtx_t *)ptr);   /* Lifetime of Rb might have ended
    ... pointer might be dangling */
    mtx_unlock((mtx_t *)ptr); /* Lifetime of Rb might have ended
       ... pointer might be dangling */
    mtx_unlock(&Ra);
}

void main(void)
{
    thrd_t id1; /* UndCC_Violation(lin, win) */
    mtx_t Rb;   /* UndCC_Violation(lin, win) */
    mtx_init(&Ra, mtx_plain);
    mtx_init(&Rb, mtx_plain);
    thrd_create(&id1, t1, &Rb);
}

#endif