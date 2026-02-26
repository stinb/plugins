
/* Code file bar.c    */
#include "foo.h"

int bar() {
    return foo();     // UndCC_Violation: foo is not declared
    // This function call will now become an implicit declaration
}

// Function called with no declaration or definition in the project
void func( void )
{
    double sq1 = power( 1, 2.0 ); // UndCC_Violation
}
