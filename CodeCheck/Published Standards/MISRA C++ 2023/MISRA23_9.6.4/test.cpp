#include <cstdlib>
#include <exception>

[[noreturn]] void kill_the_process() // UndCC_Valid
{
    std::abort(); // Note - std::abort is also [[noreturn]]
}

[[noreturn]] void throw_some() // UndCC_Valid - only exits with an exception
{
    throw 42;
}

[[noreturn]] void g(bool b)
{
    if (b)
    {
        throw std::exception{};
    }
} // UndCC_Violation - returns if 'b' is false
