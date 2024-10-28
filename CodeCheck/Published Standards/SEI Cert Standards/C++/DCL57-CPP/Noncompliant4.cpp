#if defined(_WIN32)
#include <stdexcept>

bool perform_dealloc(void *);

void operator delete(void *ptr) noexcept(false) // UndCC_Violation(Win)
{
    if (perform_dealloc(ptr))
    {
        throw std::logic_error("Something bad");
    }
}
#endif
