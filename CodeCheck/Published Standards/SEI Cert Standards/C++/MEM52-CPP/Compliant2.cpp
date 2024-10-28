#include <cstring>
#include <new>

void f(const int *array, std::size_t size) noexcept
{
    int *copy;
    try
    {
        copy = new int[size];
    }
    catch (std::bad_alloc)
    {
        // Handle error
        return;
    }
    // At this point, copy has been initialized to allocated memory
    std::memcpy(copy, array, size * sizeof(*copy));
    // ...
    delete[] copy;
}