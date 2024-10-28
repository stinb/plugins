#include <cstdint>
#include <cstddef>
void f1 ( int32_t );
void f2 ( int32_t * );
void f3 ( )
{
    f1 ( NULL );      // UndCC_Violation, NULL used as an integer
    f2 ( NULL );      // UndCC_Valid

    int32_t x = NULL; // UndCC_Violation
    int32_t y = 3;    // UndCC_Valid
}
