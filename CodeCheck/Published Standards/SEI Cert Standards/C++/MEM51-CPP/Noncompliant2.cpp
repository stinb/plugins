#include <new>

void f()
{
    int *i1, *i2;
    try
    {
        i1 = new int;
        i2 = new int;
    }
    catch (std::bad_alloc &)
    {
        delete i1;      // UndCC_Violation
        delete i2;      // UndCC_Violation
    }
}
