#include <cstdint>

class A
{
    int16_t val;
    void f()
    {
        auto a1 = [=]()               // UndCC_Violation - val is not captured, but
        { return val; };              // 'this' is implicitly captured
        auto a2 = [&]() // UndCC_Violation - val is not captured, but
        { return val; };              // 'this' is implicitly captured
        auto a3 = [this]()            // UndCC_Valid - 'this' explicitly captured
        { return val; };
        auto a4 = [self = *this]() // UndCC_Valid - current object captured by copy
        { return self.val; };
        auto i = [&]() // Rule does not apply - transient lambda
        { return val; }();
    }
};
