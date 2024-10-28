
#include <stdio.h>          // UndCC_FalseNeg
#include "usedindirect.hpp" // UndCC_FalseNeg - it could be argued that
                            // usedindirect.hpp isn't really used because we
                            // only use indirect.hpp

#include "notused.hpp"      // UndCC_Violation
#include "used.hpp"         // UndCC_Valid


int main()
{
    int i = MACUSED;
    useindirect();
    used();
    printf("Coucou\n");
    return 1;
}
