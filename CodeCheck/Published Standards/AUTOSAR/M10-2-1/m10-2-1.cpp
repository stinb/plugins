#include <cstdint>

class B1
{
public:
    int32_t count; // UndCC_Violation
    void foo();    // UndCC_Violation
    void foo1(int x);   // Exception
    void foo1(float y); // Exception
};
class B2
{
public:
    int32_t count; // UndCC_Violation
    void foo();    // UndCC_Violation
};
class D : public B1, public B2
{
public:
    void Bar()
    {
        // ++count; // Is that B1::count or B2::count?
        // foo();   // Is that B1::foo() or B2::foo()?
    }
};
