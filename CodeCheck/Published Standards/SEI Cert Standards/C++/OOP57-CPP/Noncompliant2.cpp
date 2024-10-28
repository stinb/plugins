// SEI CERT C++

#include <cstring>

class C_2n
{
    int *i;

public:
    C_2n() : i(nullptr) {}
    ~C_2n() { delete i; }

    void set(int val)
    {
        if (i)
        {
            delete i;
        }
        i = new int{val};
    }

    // ...
};

void f(C_2n &c1)
{
    C_2n c2;
    std::memcpy(&c2, &c1, sizeof(C_2n)); // UndCC_Violation
}
