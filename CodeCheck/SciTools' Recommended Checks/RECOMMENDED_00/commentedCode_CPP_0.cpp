// MISRA08_2-7-2

#include <cstdint>

void fn ( int32_t i )
{
    // ++i; // UndCC_Violation
    for ( int32_t j = 0 ; j != i ; ++j )
    {
    }
}
