#ifdef _WIN32 // C11 threading not supported on Mac or Linux
#include <stdatomic.h>
#include <threads.h>

mtx_t Ra;
mtx_t Rb;
tss_t key1;
tss_t key2;
thrd_t id1;
thrd_t id2;

int32_t t1(void *ignore) /* Thread T1 entry */
{
    /*
    ** locks/unlocks Ra, Rb
    ** accesses thread-specific storage pointed to by key1, key2
    */

    tss_delete(key1); /* UndCC_Violation(win) - might still be accessed from T2 */
}

int32_t t2(void *ignore) /* Thread T2 entry */
{
    /*
    ** locks/unlocks Ra, Rb
    ** accesses thread-specific storage pointed to by key1, key2
    */
    mtx_destroy(&Rb); /* UndCC_Violation(win) - T1 might still access Rb */
}

void main(void)
{
    mtx_init(&Ra, mtx_plain);
    mtx_init(&Rb, mtx_plain);

    tss_create(&key1, NULL);
    tss_create(&key2, NULL);

    thrd_create(&id1, t1, NULL);
    thrd_create(&id2, t2, NULL);

    spendSomeTime();
    tss_delete(key2); /* UndCC_Violation(win) - might still be accessed by t1, t2 */
    thrd_join(id1, NULL);
    thrd_join(id2, NULL);

    mtx_destroy(&Ra); /* UndCC_Valid */
    tss_delete(key1); /* UndCC_Valid */
}
#endif