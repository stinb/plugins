// Custom

#include <cstdint>

typedef signed short int16_t; // Exception
typedef int INT;              // UndCC_Violation

extern "C"                    // UndCC_Valid
{
    int foo()
    {
        return 1;
    }
}

extern int f3();              // UndCC_Violation

// Typedefs allowed in the global namespace by exception
typedef char char_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
// typedef signed long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
// typedef unsigned long uint64_t;
typedef float float32_t;
typedef double float64_t;
typedef long double float128_t;

int main()              // UndCC_Valid
{
    extern void f4();   // UndCC_Violation
}
