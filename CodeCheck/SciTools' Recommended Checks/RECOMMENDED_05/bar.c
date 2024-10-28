
/* Code file bar.c    */
#include "foo.h"

int bar() {
    return foo();     // UndCC_Violation: foo is not declared 
    // This function call will now become an implicit declaration
}
