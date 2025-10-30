#include <new>
#include <cstdint>
#include <cstddef>

auto f() noexcept
{
    return new (std::nothrow) int{42}; // UndCC_Valid
}

struct X
{
    int32_t a;
};

int32_t g()
{
    alignas(X) std::byte mem[sizeof(X)];
    X *px = new (&mem) X{1}; // UndCC_Violation - placement new
    px->~X();                // UndCC_Violation - explicit destructor call
    new (px) X{2};           // UndCC_Violation - placement new
    return px->a;            // Undefined behaviour
}

struct A
{
    void *operator new(size_t); // UndCC_Violation
};
