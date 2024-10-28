extern void c(int i, int j);
int glob;

int a()
{
    return glob + 10;
}

int b()
{
    glob = 42;
    return glob;
}

void f()
{
    c(a(), b()); // UndCC_Violation
}