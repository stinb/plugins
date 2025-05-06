#include <stdint.h>
#include <stddef.h>

extern char buffer1[12];
extern char buffer2[12];

void f1(void)
{
    (void)strcpy(buffer1, "abc");
    (void)strcpy(buffer2, "abc");
    /* The following use of memcmp is non-compliant */
    if (memcmp(buffer1, buffer2, sizeof(buffer1)) != 0) // UndCC_Violation
    {
        /*
         * The strings stored in buffer1 and buffer 2 are reported to be
         * different, but this may actually be due to differences in the
         * uninitialized characters stored after the null terminators.
         */
    }
}

/* The following definition violates other guidelines */
unsigned char headerStart[6] = {'h', 'e', 'a', 'd', 0, 164};
void f2(const uint8_t *packet)
{
    /* The following use of memcmp is compliant */
    if ((NULL != packet) && (memcmp(packet, headerStart, 6) == 0)) // UndCC_Valid
    {
        /*
         * Comparison of values having essentially unsigned type reports that
         * contents are the same. Any null terminator is simply treated as a
         * zero value and any differences beyond it are significant.
         */
    }
}
