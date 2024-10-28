// SEI CERT C++

#include <cstring>

class C_3n
{
    int i;

public:
    virtual void f();

    // ...
};

void f(C_3n &c1, C_3n &c2)
{
    if (!std::memcmp(&c1, &c2, sizeof(C_3n))) // UndCC_Violation
    {
        // ...
    }
}
