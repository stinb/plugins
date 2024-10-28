
#include <new>
#include <utility>

struct S
{
    S(const S &) noexcept; /* ... */
};

class T
{
    int n;
    S *s1;

public:
    T(T &&rhs) { *this = std::move(rhs); }
    ~T() { delete s1; }

    // ...

    void swap(T &rhs) noexcept
    {
        using std::swap;
        swap(n, rhs.n);
        swap(s1, rhs.s1);
    }

    T &operator=(T &&rhs) noexcept
    {
        using std::swap;
        swap(n, rhs.n);
        swap(s1, rhs.s1);
        return *this;
    }
};