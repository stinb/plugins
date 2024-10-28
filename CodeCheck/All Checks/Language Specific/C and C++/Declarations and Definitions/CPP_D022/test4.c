#include "test0.h"
#include "test1.h"
#include <stdio.h>

extern int global_variable3 = 0; // UndCC_Violation

int main(void)
{
    use_it();
    global_variable += 19;
    use_it();
    printf( "Increment: %d\n", increment() );
    return 0;
}
