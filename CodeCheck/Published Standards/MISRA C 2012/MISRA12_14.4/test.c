#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// MISRA12_14.4
int main() {
int32_t *p, *q;
while ( p ) /* UndCC_Violation - p is a pointer */
{
}
while ( q != NULL ) /* UndCC_Valid */
{
}
while ( true ) /* UndCC_Valid */
{
}
extern bool flag;
while ( flag ) /* UndCC_Valid */
{
}
int32_t i;
if ( i ) /* UndCC_Violation */
{
}
if ( i != 0 ) /* UndCC_Valid */
{
}

// Bitwise operators produce integer results, not boolean
int j;
if ( i & 0x0F ) { // UndCC_Violation
    j = 1;
} else {
    j = 3;
}
if ( i | 0x0F ) { // UndCC_Violation
    j = 1;
}
if ( (i & 0x0F) != 0 ) { // UndCC_Valid
    j = 1;
}
}

