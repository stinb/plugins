#include <cstring>
#include <new>

void f(const int *array, std::size_t size) noexcept
{
    int *copy = new (std::nothrow) int[size];
    if (!copy)
    {
        // Handle error
        return;
    }
    std::memcpy(copy, array, size * sizeof(*copy));
    // ...
    delete[] copy;
}