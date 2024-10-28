#include <cstddef>

void operator delete(void *ptr, std::size_t size) noexcept // UndCC_Violation
{
    delete (ptr);
}