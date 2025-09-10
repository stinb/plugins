#if defined(__linux__) || defined(_WIN32)

#include <threads.h>
#include <stdint.h>

thrd_t id1;
thrd_t id2;

int32_t t2(void *ignore) /* Thread T2 entry */
{
    ///
}

int32_t t1(void *ignore) /* Thread T1 entry */
{
    thrd_create(&id2, t2, NULL); /* UndCC_Violation , not constrained to start-up */
    ///
}

void main(void)
{
    thrd_create(&id1, t1, NULL); /* UndCC_Valid */
    ///
}

#endif