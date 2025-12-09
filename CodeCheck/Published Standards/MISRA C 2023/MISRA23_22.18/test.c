#ifdef _WIN32 // C11 threading not supported on Mac or Linux
#include <threads.h>
#include <stdint.h>

mtx_t Ra;
mtx_t Rb;

int32_t t1(void *ignore) /* Thread 1 */
{
    mtx_lock(&Rb);   /* UndCC_Valid */
    mtx_lock(&Rb);   /* UndCC_Valid - Rb is recursive */
    mtx_unlock(&Rb); /* Rb still locked */
    mtx_unlock(&Rb); /* Rb gets unlocked */
    mtx_lock(&Ra);   /* UndCC_Valid */
    mtx_lock(&Ra);   /* UndCC_Violation(Win) - undefined behaviour, deadlock possible */
    mtx_unlock(&Ra); /* If reachable (i.e. no deadlock), Ra gets unlocked */
    mtx_unlock(&Ra); /* Undefined behaviour if reachable */
    return 0;
}

thrd_t id1;
thrd_t id2;

int32_t main(void)
{
    mtx_init(&Ra, mtx_plain);
    mtx_init(&Rb, mtx_recursive);
    thrd_create(&id1, t1, NULL);
}
#endif