#include <cstdio>
#include <cstdint>

#define another_null NULL
#define im_null another_null

void f1(void)
{
    try
    {
        throw(im_null); // UndCC_Violation(Win,Lin)
    }
    catch (int32_t i) // NULL exception handled here
    {
        // ...
    }
    catch (const wchar_t *) // Developer may expect it to be caught here
    {
        // ...
    }
    wchar_t *p = NULL;
    try
    {
        throw(static_cast<const wchar_t *>(NULL)); // UndCC_Valid,
        // but breaks
        // Rule 15–0–2
        throw(p); // UndCC_Valid
    }
    catch (int32_t i)
    {
        // ...
    }
    catch (const wchar_t *) // Both exceptions handled here
    {
        // ...
    }
}
