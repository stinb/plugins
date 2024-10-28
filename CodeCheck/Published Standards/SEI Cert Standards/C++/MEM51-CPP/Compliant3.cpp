struct P
{
};

class C
{
    P *p;

public:
    C(P *p) : p(p) {}
    C(const C &) = delete;
    ~C() { delete p; }

    void operator=(const C &) = delete;

    void f() {}
};

void g(C &c)
{
    c.f();
}

void h()
{
    P *p = new P;
    C c(p);
    g(c);
}