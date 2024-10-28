#include <new>

void f()
{
    int *i1 = nullptr, *i2 = nullptr;
    try
    {
        i1 = new int;
        i2 = new int;
    }
    catch (std::bad_alloc &)
    {
        delete i1;
        delete i2;
    }
}