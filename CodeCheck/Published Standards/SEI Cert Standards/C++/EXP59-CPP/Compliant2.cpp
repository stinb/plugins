#include <cstddef>

struct S
{
    static int i;
    // ...
};
int S::i = 0;

extern void store_in_some_buffer(void *buffer, size_t offset, int val);

void f()
{
    store_in_some_buffer(&S::i, 0, 42);
}