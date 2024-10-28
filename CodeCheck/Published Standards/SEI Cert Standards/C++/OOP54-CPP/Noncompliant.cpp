#include <new>

struct S
{
    S(const S &) noexcept; /* ... */
};

class T
{
    int n;
    S *s1;

public:
    T(const T &rhs) : n(rhs.n), s1(rhs.s1 ? new S(*rhs.s1) : nullptr) {}
    ~T() { delete s1; }

    // ...

    T &operator=(const T &rhs)  // UndCC_Violation
    {
        n = rhs.n;
        delete s1;
        s1 = new S(*rhs.s1);
        return *this;
    }
};
