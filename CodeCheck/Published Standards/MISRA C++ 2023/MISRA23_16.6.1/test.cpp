#include <cstdint>
#include <ostream>
#include <iostream>

class C
{
    int32_t i;

public:
    C(int32_t x = 0) : i(x) {}
    C operator+(C rhs) const;         // UndCC_Violation
    C operator+(int32_t rhs) const;   // UndCC_Violation
    C operator and(C rhs) const;      // UndCC_Violation
    friend C operator*(C lhs, C rhs); // UndCC_Valid - non-member friend
    friend C operator-(C lhs, C rhs)  // UndCC_Valid - hidden friend
    {
        return C(lhs.i - rhs.i);
    }
    friend std::ostream &
    operator<<(std::ostream &os,
               C const &c);      // Rule does not apply - not symmetrical
    C &operator/=(C const &rhs); // Rule does not apply - not symmetrical
};

C operator/(C lhs, C rhs); // UndCC_Valid - non-member
C operator*(C lhs, C rhs); // UndCC_Valid - non-member friend

int main()
{
    C c(21);
    std::cout << (c + 1) << '\n';
    // std::cout << ( 1 + c ) << '\n'; // Would fail to compile
    std::cout << (c * 4) << '\n';
    std::cout << (4 * c) << '\n';
    std::cout << (84 / c) << '\n';
}
