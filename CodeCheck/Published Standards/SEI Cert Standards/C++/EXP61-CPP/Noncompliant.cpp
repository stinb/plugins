auto g()
{
    int i = 12;
    return [&]
    {
        i = 100;
        return i; // UndCC_Violation
    };
}

void f()
{
    int j = g()();
}