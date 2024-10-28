class S
{
    int v;

public:
    S() : v(12) {} // Non-trivial constructor

    void f();
};

void f()
{

    // ...
#if defined(_WIN32)
    goto bad_idea; // Error in strict parser
#endif
    // ...

    S s; // Control passes over the declaration, so initialization does not take place.

bad_idea:
    s.f();
}
