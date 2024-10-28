#include <exception>

int f_helper() noexcept
{
    try
    {
        extern int f() noexcept(false);
        return f();
    }
    catch (...)
    {
        extern void log_message(const char *) noexcept;
        log_message("f() threw an exception");
        std::terminate();
    }
    // Unreachable.
}

int i = f_helper();

int main()
{
    // ...
}