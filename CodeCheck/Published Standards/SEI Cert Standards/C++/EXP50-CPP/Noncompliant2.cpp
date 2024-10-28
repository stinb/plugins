void f(int i, const int *b)
{
    int a = i + b[++i]; // UndCC_Violation
    // ...
}