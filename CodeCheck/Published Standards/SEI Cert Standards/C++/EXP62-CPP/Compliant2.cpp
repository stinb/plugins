struct S
{
    int i, j, k;

    // ...

    virtual void f();
    void clear() { i = j = k = 0; }
};

void f()
{
    S *s = new S;
    // ...
    s->clear();
    // ...
    s->f(); // ok
}