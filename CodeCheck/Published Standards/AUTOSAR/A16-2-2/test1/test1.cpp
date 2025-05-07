#include <cstdint>
#include <math.h>           // UndCC_Valid - sqrt (indirect on win32)
#include <stdio.h>          // UndCC_Valid - printf
#include "usedindirect.hpp" // UndCC_Valid - useindirect (indirect)
#include "notused.hpp"      // UndCC_Violation
#include "used.hpp"         // UndCC_Valid


int main()
{
    int32_t x;
    sqrt(100.0);
    int i = MACUSED;
    useindirect();
    used();
    printf("Coucou\n");
    return 1;
}
