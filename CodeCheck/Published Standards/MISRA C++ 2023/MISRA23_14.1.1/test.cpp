#include <cstdint>

class C1 // UndCC_Violation - has public and private members
{
public:
    int32_t a;

private:
    int32_t b;
};

struct C2 // UndCC_Valid
{
    C2(int32_t a, int32_t b) : a{a}, b{b} {}
    int32_t a;
    int32_t b;
};

class C3 // UndCC_Valid - rule does not apply to static members
{
public:
    C3(int32_t a, int32_t b) : a{a}, b{b} {}
    static int32_t s;

private:
    int32_t a;
    int32_t b;
};