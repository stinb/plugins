#include <cstring>

void f(const int *array, std::size_t size) noexcept
{
    int *copy = new int[size]; // UndCC_Violation
    std::memcpy(copy, array, size * sizeof(*copy));
    // ...
    delete[] copy;
}
