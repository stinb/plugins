#include <cstdint>

void f1()
{
    int8_t *p;
    {
        int8_t local;
        int8_t local_array[10];
        p = &local;      // UndCC_Violation
        p = local_array; // UndCC_Violation
    }
}

void f2()
{
    int8_t *p1;
    {
        int8_t *p2 = nullptr;
        int8_t local;
        p2 = &local; // UndCC_Valid - objects have the same lifetime
        p1 = p2;     // Rule does not apply
    }
    *p1 = 0; // Undefined behaviour
}

void f3()
{
    int8_t *p;
    int8_t local;
    {
        int8_t local_array[10];
        p = &local;      // UndCC_Valid
        p = local_array; // UndCC_Violation
    }
}