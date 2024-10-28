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
    int a_val, b_val;

    a_val = a();
    b_val = b();

    c(a_val, b_val);
}