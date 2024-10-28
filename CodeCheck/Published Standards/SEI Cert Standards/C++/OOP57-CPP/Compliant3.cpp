// SEI CERT C++

class C_3
{
    int i;

public:
    virtual void f();

    bool operator==(const C_3 &rhs) const
    {
        return rhs.i == i;
    }

    // ...
};

void f(C_3 &c1, C_3 &c2)
{
    if (c1 == c2)
    {
        // ...
    }
}
