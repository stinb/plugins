void g(int &i)
{
    i = 42;
}

void f()
{
    int i = 4;
    g(i);
}