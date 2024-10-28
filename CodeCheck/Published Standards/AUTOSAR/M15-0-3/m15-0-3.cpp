// MISRA C++ 2008

#include <cstdint>

#ifdef __UNDERSTAND_FUZZY_PARSE__

void f(int32_t i)
{
    if (10 == i)
    {
        goto Label_10; // UndCC_Violation
    }

    if (11 == i)
    {
        goto Label_11; // UndCC_Violation
    }

    switch (i)
    {
    case 1:
        try
        {
        Label_10:
        case 2: // UndCC_Violation – also violates switch rules
            // Action
            break;
        }
        catch (...)
        {
        Label_11:
        case 3: // UndCC_Violation – also violates switch rules
            // Action
            break;
        }
        break;
    default:
    {
        // Default Action
        break;
    }
    }
}

#endif
