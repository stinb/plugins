void f(int i, const int *b)
{
    ++i;
    int a = i + b[i];
    // ...
}