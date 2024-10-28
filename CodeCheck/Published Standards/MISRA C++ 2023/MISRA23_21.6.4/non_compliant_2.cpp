#include <cstddef>
#include <cstdlib>

void operator delete(void *ptr) noexcept // UndCC_Violation
{
    std::free(ptr);
}