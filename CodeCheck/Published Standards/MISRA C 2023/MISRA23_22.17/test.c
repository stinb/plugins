#if defined(__linux__) || defined(_WIN32)

#include <threads.h>
#include <stdint.h>

mtx_t Ra;
mtx_t Rb;
cnd_t Cnd1;
cnd_t Cnd2;

int32_t t1(void *ignore) /* Thread 1 */
{
    mtx_lock(&Ra);
    mtx_unlock(&Ra);      /* UndCC_Valid */
    mtx_unlock(&Ra);      /* UndCC_Violation - mutex is not locked */
    cnd_wait(&Cnd1, &Ra); /* UndCC_Violation - mutex is not locked */
    mtx_unlock(&Rb);      /* UndCC_Violation - mutex either not locked, or
        ... is locked by different thread */
    cnd_wait(&Cnd2, &Rb); /* UndCC_Violation - mutex either not locked, or
 ... is locked by different thread */
    return 0;
}

int32_t t2(void *ignore) /* Thread 2 */
{
    mtx_lock(&Rb);
    doSomething();
    mtx_unlock(&Rb); /* UndCC_Valid */
    return 0;
}

#endif