// SEI CERT C++

#include <iostream>
#include <utility>

class C
{
    int scalingFactor;
    int otherData;

public:
    C() : scalingFactor(1) {}

    void set_other_data(int i);
    int f(int i)
    {
        return i / scalingFactor;
    }
    // ...
};

template <typename T>
T &clear(T &o)
{
    using std::swap;
    T empty;
    swap(o, empty);
    return o;
}

void f()
{
    C c;

    // ... Code that mutates c ...

    // Reinitialize c to its default state
    clear(c);

    std::cout << c.f(100) << std::endl;
}
