#include <threads.h>
#include <stdint.h>

mtx_t Ra;
mtx_t Rb;
mtx_t Rc;

struct timespec *ts;

void main(void)
{
    mtx_init(&Ra, mtx_plain);
    mtx_init(&Rb, mtx_timed);
    mtx_init(&Rc, mtx_timed | mtx_recursive);
}

int32_t t1(void *ignore)
{
    mtx_timedlock(&Ra, ts); /* UndCC_Violation(lin, win) */
    mtx_timedlock(&Rb, ts); /* UndCC_Valid */
    mtx_timedlock(&Rc, ts); /* UndCC_Valid */
}
