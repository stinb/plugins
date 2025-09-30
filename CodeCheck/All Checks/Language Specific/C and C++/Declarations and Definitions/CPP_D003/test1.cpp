#include <stdint.h>

       int32_t array1[ 10 ];              // UndCC_Valid
extern int32_t array2[ ];                 // UndCC_Violation
       int32_t array3[ ] = { 0, 10, 15 }; // UndCC_Valid
extern int32_t array4[ 42 ];              // UndCC_Valid
