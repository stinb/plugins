#include <stdexcept>


/// @checkedException
class Error1: public std::exception
{
    // Implementation
};
/// @checkedException
class Error2: public std::exception
{
    // Implementation
};
/// @checkedException
class Error3: public std::exception
{
    // Implementation
};


/// @throw Error1 Error 1
/// @throw Error2 Error 2
void f(float x) noexcept(false) // UNDCC_Valid - Both errors are thrown
{
    throw Error1();
    throw Error2();
}

/// @throw Error1 Error 1
void f(int x) noexcept(false)   // UNDCC_Violation - Error 2 not declared
{
    throw Error1();
    throw Error2();
}

/// @throw Error1 Error 1
/// @throw Error2 Error 2
/// @throw Error3 Error 3
void f() noexcept(false)        // UNDCC_Violation - Error 3 declared for no reason
{
    throw Error1();
    throw Error2();
}
