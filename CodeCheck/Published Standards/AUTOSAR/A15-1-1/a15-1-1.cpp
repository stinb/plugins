//% $Id: A15-1-1.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <memory>
#include <stdexcept>

class A
{
    // Implementation
};

class MyException : public std::logic_error
{
public:
    using std::logic_error::logic_error;
    // Implementation
};

void F1()
{
    throw -1; // UndCC_Violation - integer literal thrown
}

void F2()
{
    throw nullptr; // UndCC_Violation - null-pointer-constant thrown
}

void F3()
{
    throw A(); 
    // std::exception thrown
}

void F4()
{
    throw std::logic_error{
        "Logic Error"}; // UndCC_Valid - std library exception thrown
}

void F5()
{
    throw MyException{"Logic Error"}; // UndCC_Valid - user-defined type that
    // inherits from std::exception thrown
}

void F6()
{
    throw std::make_shared<std::exception>( // UndCC_Violation
        std::logic_error("Logic Error")); 
    // not inherit from std::exception
}

void F7()
{
    try
    {
        F6();
    }

    catch (std::exception &e) // An exception of
    // std::shared_ptr<std::exception> type will not
    // be caught here
    {
        // Handle an exception
    }
    catch (std::shared_ptr<std::exception> &e) // An exception of
    // std::shared_ptr<std::exception>
    // type will be caught here, but
    // unable to access
    // std::logic_error information
    {
        // Handle an exception
    }
}