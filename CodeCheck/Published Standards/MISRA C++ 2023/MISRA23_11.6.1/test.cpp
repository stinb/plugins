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
int32_t h();
void catchClause()
{
    try
    {
        throw 1;
    }
    catch (int32_t e1) // UndCC_Valid - initialized from the exception object
    {
        int32_t inCatch;       // UndCC_Violation
        int32_t inCatchOk = e1; // UndCC_Valid
    }
    catch (const char *e2) // UndCC_Valid - initialized from the exception object
    {
    }
    catch (...)
    {
    }
}
void selectionStatements(bool cond)
{
    if (int32_t c1 = h()) // UndCC_Valid - condition requires an initializer
    {
        int32_t inIf; // UndCC_Violation
    }
    if (int32_t c2 = h(); c2 > 0) // UndCC_Valid
    {
    }
    if (int32_t c3; cond) // UndCC_Violation
    {
    }
    switch (int32_t s1; cond) // UndCC_Violation
    {
    default:
        break;
    }
    while (int32_t w1 = h()) // UndCC_Valid
    {
    }
}
