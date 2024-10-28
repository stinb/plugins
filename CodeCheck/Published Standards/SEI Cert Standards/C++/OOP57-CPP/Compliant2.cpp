// SEI CERT C++

class C_2
{
    int *i;

public:
    C_2() : i(nullptr) {}
    ~C_2() { delete i; }

    void set(int val)
    {
        if (i)
        {
            delete i;
        }
        i = new int{val};
    }

    C_2 &operator=(const C_2 &rhs) noexcept(false)
    {
        if (this != &rhs)
        {
            int *o = nullptr;
            if (rhs.i)
            {
                o = new int;
                *o = *rhs.i;
            }
            // Does not modify this unless allocation succeeds.
            delete i;
            i = o;
        }
        return *this;
    }

    // ...
};

void f(C_2 &c1)
{
    C_2 c2 = c1;
}
