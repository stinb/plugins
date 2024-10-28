// A15-4-4 (modified)
#include <iostream>
#include <stdexcept>
void F1(); // UndCC_Violation(1) - missing noexcept specification
void F2() noexcept;        // UndCC_Valid - f2 does not throw exceptions
void F3() noexcept(true);  // UndCC_Valid - f3 does not throw exceptions
void F4() noexcept(false); // UndCC_Valid - f4 declares to throw exceptions
void F5() noexcept         // UndCC_Valid - f5 does not throw exceptions
{
    try
    {
        F1(); // Exception handling needed, f1 has no noexcept specification
    }

    catch (std::exception& e)
    {
        // Handle exceptions
    }

    F2(); // Exception handling not needed, f2 is noexcept
    F3(); // Exception handling not needed, f3 is noexcept(true)

    try
    {
        F4(); // Exception handling needed, f4 is noexcept(false)
    }

    catch (std::exception& e)
    {
        // Handle exceptions
    }
}
template <class T>
void F6() noexcept(noexcept(T())); // UndCC_Valid - function f6() may be
                                   // noexcept(true) or noexcept(false)
                                   // depending on constructor of class T
template <class T>
class A
{
    public:
        A() noexcept(noexcept(T())) // UndCC_Valid - constructor of class A may be
        // noexcept(true) or noexcept(false) depending on
        // constructor of class T
        {
        }
};
class C1
{
    public:
        C1()
            noexcept(
                    true) // UndCC_Valid - constructor of class C1 does not throw exceptions
            {
            }
        // ...
};
class C2
{
    public:
        C2() // UndCC_Violation(1) - missing noexcept specification
        {
        }
        // ...
};
void F7() noexcept // UndCC_Valid - f7 does not throw exceptions
{
    std::cout << noexcept(A<C1>()) << '\n'; // prints '1' - constructor of
                                            // A<C1> class is noexcept(true)
                                            // because constructor of C1 class
                                            // is declared to be noexcept(true)
    std::cout << noexcept(A<C2>()) << '\n'; // prints '0' - constructor of
                                            // A<C2> class is noexcept(false)
                                            // because constructor of C2 class
                                            // has no noexcept specifier
}
