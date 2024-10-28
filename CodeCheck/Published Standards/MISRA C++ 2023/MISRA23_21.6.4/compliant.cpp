#include <cstddef>
#include <cstdlib>

void operator delete(void *ptr) noexcept // UndCC_Valid
{
    std::free(ptr);
}

void operator delete(void *ptr, std::size_t size) noexcept // UndCC_Valid
{
    delete (ptr);
}