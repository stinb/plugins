#include <threads.h> // C11 threads not supported by Mac/Linux
#include <stdint.h>

typedef int bool_t;

mtx_t Ra;
mtx_t Rb;

int32_t t1(void *ignore) /* Thread 1 */
{
    bool_t b;

    mtx_lock(&Ra); /* UndCC_Valid */
    mtx_unlock(&Ra);

    mtx_lock(&Rb); /* UndCC_Violation(Win) - unlock missing on one path */

    if (b)
    {
        mtx_unlock(&Rb);
    }

    return 0;
}
