#include <fenv.h>

typedef float float32_t;

void f(float32_t x, float32_t y)
{
    float32_t z;
    feclearexcept(FE_DIVBYZERO); /* UndCC_Violation */
    z = x / y;
    if (fetestexcept(FE_DIVBYZERO)) /* UndCC_Violation */
    {
    }
    else
    {
#pragma STDC FENV_ACCESS ON
        z = x * y;
    }
    if (z > x)
    {
#pragma STDC FENV_ACCESS OFF
        if (fetestexcept(FE_OVERFLOW)) /* UndCC_Violation */
        {
        }
    }
}
