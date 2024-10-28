struct S
{
    ~S();
};

void f()
{
    S *s = new S();
    // ...
    delete s;
}