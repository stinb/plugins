#include <assert.h>
#include <stddef.h>

void process(size_t index)
{
    assert(index++ > 0); /* UndCC_Violation, Side effect */
                         /* ... */
}