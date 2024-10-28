#include <cstdint>

class B
{
};

class D : public virtual B // UndCC_Violation
{
};

class E : public B
{
}; // UndCC_Valid

struct F
{
    virtual int32_t foo() { return 1; }
};

struct G : public virtual F // UndCC_Violation
{
    int32_t goo()
    {
        return foo();
    }
};

struct H : public virtual F // UndCC_Violation
{
    int32_t foo() override { return 2; }
};

struct I : H, G
{
};

int main()
{
    I i;
    return i.goo(); // Calls C::foo(), which may not be expected
}
