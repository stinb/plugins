#include <cstdlib>
#include <stdexcept>

bool perform_dealloc(void *);
void log_failure(const char *);

void operator delete(void *ptr) noexcept(true)
{
    if (perform_dealloc(ptr))
    {
        log_failure("Deallocation of pointer failed");
        std::exit(1); // Fail, but still call destructors
    }
}