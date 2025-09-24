// Header file a.h
#include <cstdint>

void f1();          // Rule does not apply - not a definition
void f2() {}        // UndCC_Violation
inline void f3() {} // UndCC_Valid

template <typename T>
void f4(T) {} // UndCC_Valid - implicitly inline

int32_t a;                       // UndCC_Violation
constexpr auto ans_1{42};        // UndCC_Valid - no external linkage
extern constexpr auto ans_2{42}; // UndCC_Violation - external linkage

struct X
{
    int32_t a;                        // UndCC_Valid - no linkage
    static int32_t b;                 // UndCC_Valid
    static const int32_t c{0};        // UndCC_Valid - const
    inline static const int32_t d{2}; // UndCC_Valid - X::b has external linkage but is inline
};
