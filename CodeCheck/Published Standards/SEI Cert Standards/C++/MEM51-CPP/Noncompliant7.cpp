#include <cstdlib>
#include <new>

struct S
{
    static void *operator new(std::size_t size) noexcept(true)
    {
        return std::malloc(size);
    }

    static void operator delete(void *ptr) noexcept(true)
    {
        std::free(ptr);
    }
};

void f()
{
    S *s = new S;
    ::delete s; // UndCC_Violation
}
