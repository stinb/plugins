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
while ( flag ) /* UnddCC_Valid */
{
}
int32_t i;
if ( i ) /* UndCC_Violation */
{
}
if ( i != 0 ) /* UndCC_Valid */
{
}
}

