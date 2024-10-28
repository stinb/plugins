
class S
{
    int v;

public:
    S() : v(12) {} // Non-trivial constructor

    void f();
};

void f()
{
    S s;

    // ...

    goto bad_idea;

    // ...

bad_idea:
    s.f();
}