#include "m15-1-1.h"

class E
{
public:
    E() {} // Assume constructor cannot cause an exception
    void err_throw()
    {
        throw 0;
    }
};

void f1(void)
{
    try
    {
        if (true)
        {
            throw E(); // UndCC_Valid – no exception thrown
        }              // when constructing the object
    }
    catch (int)
    {
    }
}

// construction of E2 causes an exception to be thrown
// class E2
// {
// public:
//     E2()
//     {
//         throw 10;
//     }
// };

void f2(void)
{
    try
    {
        if (true)
        {
            throw E2(); // UndCC_Violation – int exception thrown
            // when constructing the E2 object
        }
    }
    catch (int)
    {
    }
}
