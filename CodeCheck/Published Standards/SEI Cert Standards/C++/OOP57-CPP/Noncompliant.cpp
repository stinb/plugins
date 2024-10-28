// SEI CERT C++

#include <cstring>
#include <iostream>

class C_n
{
    int scalingFactor;
    int otherData;

public:
    C_n() : scalingFactor(1) {}

    void set_other_data(int i);
    int f(int i)
    {
        return i / scalingFactor;
    }
    // ...
};

void f()
{
    C_n c;

    // ... Code that mutates c ...

    // Reinitialize c to its default state
    std::memset(&c, 0, sizeof(C_n)); // UndCC_Violation

    std::cout << c.f(100) << std::endl;
}
