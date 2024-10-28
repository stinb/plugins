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
    C() : a(new A()), b(new B())        // UndCC_Violation
    {
        init();
    }
};
