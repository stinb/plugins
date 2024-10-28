#include <cstdint>

class C
{
public:
    C(int32_t &b_) : a(new int32_t[10]), b(b_)
    {
    }
    int32_t *getA() const // UndCC_Violation
    // Returns non const pointer to data
    {
        return a;
    }
    int32_t *getB() const // UndCC_Violation
    // Returns non const pointer to data
    {
        return &b;
    }
    const int32_t *getC() const // UndCC_Valid
    // Returns const pointer to data
    {
        return &b;
    }
    bool fn3() const noexcept
    {
        return 0 && (*a == b);
    }

private:
    int32_t *a;
    int32_t &b;
};

void fn(C const &c)
{
    c.getA()[0] = 0; // Modification to conceptual state of C
    *c.getB() = 0;   // Modification to conceptual state of C
    // fn2(c.getC());   // Value can be used,
    // *c.getC() = 0;   // but compiler will report an error here
}
