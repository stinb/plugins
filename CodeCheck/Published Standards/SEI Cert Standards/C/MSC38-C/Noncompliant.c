#include <assert.h>

typedef void (*handler_type)(int);
extern int errno; // UNDCC_Violation - should be included as a header, <errno.h>


void execute_handler(handler_type handler, int value)
{
    handler(value);
}

void func(int e)
{
    // execute_handler(&(assert), e < 0); // Analysis error
}
