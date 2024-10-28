#include <setjmp.h> // UndCC_Violation

jmp_buf jump;

int foo(int n)
{
    if (n == 0)
        goto zero; // UndCC_Violation - use of goto
    else
        return n;

zero:
    return 0;
}

void foo_1()
{
    setjmp(jump);
}

void foo_2()
{
    longjmp(jump, 1);
}

int foo_3(int n)
{
    if (n < 10)
    {
        n += n;
        return foo_3(n); // UndCC_Violation - direct recursion
    }
    return n += n;
}

int foo_5(int n);

int foo_4(int n)
{
    if (n < 10)
        return foo_5(n); // UndCC_Violation - indirect recursion

    return n;
}

int foo_5(int n)
{
    n *= 2;
    return foo_4(n); // UndCC_Violation - indirect recursion
}
