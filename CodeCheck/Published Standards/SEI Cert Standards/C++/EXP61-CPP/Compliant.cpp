auto g()
{
    int i = 12;
    return [=]() mutable
    {
        i = 100;
        return i;
    };
}

void f()
{
    int j = g()();
}