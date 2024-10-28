//% $Id: A18-5-11.cpp 316977 2018-04-20 12:37:31Z christof.meerwald $
#include <cstdlib>

void *operator new[](std::size_t s, int x);     // UndCC_Violation

class A
{
public:
    static void *operator new[](std::size_t s); // UndCC_Valid: operator new
    static void operator delete[](void *ptr);   // defined together with operator delete
};

class B
{
public:
    static void *operator new(std::size_t s);   // UndCC_Violation: operator
    static void *operator new[](std::size_t s); // new defined without // UndCC_Violation
                                                // corresponding operator delete
};
