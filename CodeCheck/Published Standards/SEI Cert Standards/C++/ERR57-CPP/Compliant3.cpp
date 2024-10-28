struct A
{ /* ... */
};
struct B
{ /* ... */
};

class C
{
    A *a;
    B *b;

protected:
    void init() noexcept(false);

public:
    C() : a(nullptr), b(nullptr)
    {
        try
        {
            a = new A();
            b = new B();
            init();
        }
        catch (...)
        {
            delete a;
            delete b;
            throw;
        }
    }
};