#include <algorithm>
#include <vector>

class A
{
    mutable int m;

public:
    A() : m(0) {}
    explicit A(int m) : m(m) {}

    A(const A &other) : m(other.m)
    {
        other.m = 0; // UndCC_Violation
    }

    A &operator=(const A &other)
    {
        if (&other != this)
        {
            m = other.m;
            other.m = 0; // UndCC_Violation
        }
        return *this;
    }

    int get_m() const { return m; }
};

void f()
{
    std::vector<A> v{10};
    A obj(12);
    std::fill(v.begin(), v.end(), obj);
}
