void foo()
{
    int a, b, c, d;
    int *ptr;
    char u8_a;

    if ((a < b) && (c < d)) // UndCC_Valid
    {
    }
    if (1 && (c < d)) // UndCC_Violation
    {
    }
    if ((a < b) && (c + d)) // UndCC_Violation
    {
    }
    if (u8_a && (c + d)) // UndCC_Violation
    {
    }
    if (!0) // UndCC_Violation – also breaks other rules
    {
    }
    if (!ptr) // UndCC_Violation
    {
    }
    if (!false) // UndCC_Valid with this rule, but breaks others
    {
    }
}