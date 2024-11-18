#include <stdint.h>
#include <stdalign.h>
#include <threads.h>

thrd_t id1;
thrd_t id2;

_Atomic int32_t g_ai1; /* UndCC_Valid - default initialization */

int32_t t1(int32_t *ptr)
{
    /* accesses g_ai1, ai1, ai2, ai3, ai4 */
}

void main(void)
{
    _Atomic int32_t ai1 = 22; /* UndCC_Valid - directly initialized */
    _Atomic int32_t ai2;
    ai2 = 777; /* UndCC_Violation - not initialized by atomic_init */
    _Atomic int32_t ai3;
    atomic_init(&ai3, 333); /* UndCC_Valid - Initialized by atomic_init */
    /* ------------ */

    _Atomic int32_t ai4;
    thrd_create(&id1, t1, &ai4); /* UndCC_Violation */

    atomic_init(&ai4, 666); /*  Initialized after user-thread T1 is created */

    thrd_join(id1, NULL);
}
