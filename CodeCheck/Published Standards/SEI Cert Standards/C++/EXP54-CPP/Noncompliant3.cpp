int *g()
{
    int i = 12;
    return &i;
}

void h(int *i);

void f()
{
    int *i = g(); // UndCC_Violation
    h(i);
}
