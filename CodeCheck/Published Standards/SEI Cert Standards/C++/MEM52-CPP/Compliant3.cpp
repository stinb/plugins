#include <cstring>

void f(const int *array, std::size_t size) noexcept(false)
{
    int *copy = new int[size];
    // If the allocation fails, it will throw an exception which the caller
    // will have to handle.
    std::memcpy(copy, array, size * sizeof(*copy));
    // ...
    delete[] copy;
}