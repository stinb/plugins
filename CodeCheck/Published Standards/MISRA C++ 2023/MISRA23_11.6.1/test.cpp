#include <cstdint>
#include <string>

void f(bool cond)
{
    int32_t i; // UndCC_Violation
    if (cond)
    {
        i = 42;
    }
    else
    {
        i = -1;
    }
    int32_t j = cond ? 42 : -1; // UndCC_Valid
    int32_t k = [&]()           // UndCC_Valid
    {
        if (cond)
        {
            return 42;
        }
        else
        {
            return -1;
        }
    }();
    std::string s; // UndCC_Valid - default-initialized
}
int32_t g; // UndCC_Valid - static initialization applies
void f()
{
    thread_local int32_t i; // UndCC_Valid - static initialization applies
}
