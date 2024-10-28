// Custom

class C
{
    unsigned char buffType;
    int size;

    friend bool operator==(const C &lhs, const C &rhs)
    {
        return lhs.buffType == rhs.buffType &&
               lhs.size == rhs.size;
    }
};

void f(const C &c1, const C &c2)
{
    if (c1 == c2)
    {
        // ...
    }
}

class D
{
    int i, j, k;

    // ...
public:
    virtual void f();
    void clear() { i = j = k = 0; }
};

void f()
{
    D *d = new D;
    // ...
    d->clear();
    // ...
    d->f(); // ok
}
