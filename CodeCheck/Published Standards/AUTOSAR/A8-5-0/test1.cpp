// MISRA C++ 2023 Rule 11.6.2 - Example from standard

#include <cstdint>

namespace {
  int32_t Z; // UndCC_Valid - implicitly initialized with '0'
}

void f()
{
    int32_t i;
    int32_t j = i + 1;    // UndCC_Violation - i has not been assigned a value

    int32_t *p = new int32_t;
    int32_t k = *p;       // UndCC_Violation - *p has not been assigned a value

    int32_t *q;
    if ( q == p )          // UndCC_Violation - q has not been assigned a value
    {
    }
}

int32_t g( bool b )
{
    if ( b )
    {
        goto L1;
    }
    int32_t x;
    x = 10u;
L1:
    x = x + 1u;           // UndCC_FalseNeg - x may not have been assigned a value
    return x;
}

struct S
{
    int32_t a;
    int32_t b;
};

void h()
{
    S s1;
    S s2 = { 10 };
    auto i1 = s1.a;       // UndCC_FalseNeg
    auto i2 = s2.b;       // UndCC_Valid - s2.b implicitly initialized to 0

    int32_t array1[ 10 ] = { 1, 2, 3 };
    int32_t array2[ 10 ];
    auto i3 = array1[ 5 ]; // UndCC_Valid - array1[ 5 ] implicitly initialized to 0
    auto i4 = array2[ 5 ]; // UndCC_Violation
}

class C
{
public:
    C() : m_a( 10 ), m_b( 7 ) // Both m_a and m_b initialized
    {
    }
    C( int32_t a ) : m_a( a ) // m_b not initialized
    {
    }
    int32_t GetmB() { return m_b; }

private:
    int32_t m_a;
    int32_t m_b;
};

int main()
{
    C c1;
    if ( c1.GetmB() > 0 )     // UndCC_Valid - m_b initialized
    {
    }
    C c2( 5 );
    if ( c2.GetmB() > 0 )     // UndCC_FalseNeg - m_b not initialized
    {
    }
}
