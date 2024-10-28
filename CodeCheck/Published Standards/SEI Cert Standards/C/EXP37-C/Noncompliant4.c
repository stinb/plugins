/* In this source file, no f prototype in scope */

long f(); // UndCC_Violation

long g(int x)
{
    return f(x);
}