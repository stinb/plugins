#include "Bad.h"

class SomeClass
{
    Bad bad_member;

public:
    ~SomeClass() // UndCC_Violation
    try
    {
        // ...
    }
    catch (...)
    {
        // Handle the exception thrown from the Bad destructor.
    }
};