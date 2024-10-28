#include <cstddef>

struct S
{
    static int i;
    // ...
};
int S::i = 0;

extern void store_in_some_buffer(void *buffer, size_t offset, int val);
extern void *buffer;

void f()
{
    // size_t off = offsetof(S, i); //  Invalid - error in strict parser
    // store_in_some_buffer(buffer, off, 42);
}
