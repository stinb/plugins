void g(const int &ci)
{
    int &ir = const_cast<int &>(ci); // UndCC_Violation
    ir = 42;
}

void f()
{
    const int i = 4;
    g(i);
}