//% $Id: A15-4-1.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <stdexcept>
void F1() noexcept; // UndCC_Valid - note that noexcept is the same as
// noexcept(true)
void F2() throw(); // UndCC_Violation - dynamic exception-specification is
// deprecated
void F3() noexcept(false);           // UndCC_Valid
void F4() throw(std::runtime_error); // UndCC_Violation - dynamic
// exception-specification is deprecated
void F5() throw(std::exception); // UndCC_Violation - dynamic exception-specification is deprecated
template <class T>
void F6() noexcept(noexcept(T())); // UndCC_Valid

struct B
{
    B() throw(); // UndCC_Violation
    B(const B &) = default; // exception specification is "noexcept(true)"
    B(B &&, int = (throw 1, 0)) noexcept; // UndCC_Violation
    ~B() throw(); // UndCC_Violation
};