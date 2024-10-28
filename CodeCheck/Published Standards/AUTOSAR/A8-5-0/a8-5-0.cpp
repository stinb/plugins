// $Id: A8-5-0.cpp 307536 2018-02-14 12:35:11Z jan.babst $
#include <cstdint>
#include <string>

static std::int32_t zero; // UndCC_Valid - Variable with static storage duration
// is zero-initialized.

void local(int x)
{
    std::int32_t a1;   // No initialization
    std::int32_t b1{}; // UndCC_Valid - zero initialization

    b1 = a1;   // UndCC_Violation - uninitialized memory read
    a1 = zero; // UndCC_Valid - a is zero now
    b1 = a1;   // UndCC_Valid - read from initialized memory

    std::string s1; // UndCC_Valid - default constructor is a called
    // read from s
}

void dynamic()
{
    // Note: These examples violate A18-5-2

    auto const a2 = new std::int32_t;   // No initialization
    auto const b2 = new std::int32_t{}; // UndCC_Valid - zero initialization

    *b2 = *a2;  // UndCC_Violation - uninitialized memory read
    *a2 = zero; // UndCC_Valid - a is zero now
    *b2 = *a2;  // UndCC_Valid - read from initialized memory

    delete b2;
    delete a2;

    auto const s2 =
        new std::string; // UndCC_Valid - default constructor is a called
    // read from *s
    delete s2;
}

// Members of Bad are default-initialized by the (implicitly generated) default
// constructor. Note that this violates A12-1-1.
struct Bad
{
    std::int32_t a;
    std::int32_t b;
};

void foo(Bad b)
{
    Bad b1;
    Bad b2;
    Bad b3;

    b3.a = b.a;
    b3.a = b2.a;
}

// UndCC_Valid - Members of Good are explicitly initialized.
// This also complies to A12-1-1.
struct Good
{
    std::int32_t a = 0;
    std::int32_t b{0};
};

void members(Bad c)
{
    Bad bad; // Default constructor is called, but members a not initialized

    bad.b = bad.a;   
    bad.a = zero;  // UndCC_Valid - bad.a is zero now
    bad.b = bad.a; // UndCC_Valid - read from initialized memory

    Good good; // Default constructor is called and initializes members

    std::int32_t x = good.a; // UndCC_Valid
    std::int32_t y = good.b; // UndCC_Valid
}

// int foo1()
// {
//     int count = 0;
//     for (int i = 0; i < count; ++i)
//     {
//         if (true)
//             return i;
//     }

//     while (count)
//     {
//         count--;
//     }

//     return -1;
// }

// class C1
// {
// public:
//     int num = 0;
// };

// class C2
// {
// public:
//     C1 c1;
// };

// void foo2()
// {
//     C2 c2;
//     C1 c1_2 = c2.c1;
// }
