// AUTOSAR C++ 2014

#include <cstdint>
using IntPtr = std::int8_t *;
struct S
{
    std::int8_t* s1;   // UndCC_Valid
    std::int8_t** s2;  // UndCC_Valid
    std::int8_t*** s3; // UndCC_Violation
};
S* ps1;   // UndCC_Valid
S** ps2;  // UndCC_Valid
S*** ps3; // UndCC_Violation

std::int8_t** (*pfunc1)();   // UndCC_Valid
std::int8_t** (**pfunc2)();  // UndCC_Valid
std::int8_t** (***pfunc3)(); // UndCC_Violation
std::int8_t*** (**pfunc4)(); // UndCC_Violation

void Fn(std::int8_t* par1,         // UndCC_Valid
        std::int8_t** par2,        // UndCC_Valid
        std::int8_t*** par3,       // UndCC_Violation
        IntPtr* par4,              // UndCC_Valid
        IntPtr* const* const par5, // UndCC_Violation
        std::int8_t* par6[],       // UndCC_Valid
        std::int8_t** par7[])      // UndCC_Violation
{
    std::int8_t* ptr1;                   // UndCC_Valid
    std::int8_t** ptr2;                  // UndCC_Valid
    std::int8_t*** ptr3;                 // UndCC_Violation
    IntPtr* ptr4;                        // UndCC_Valid
    IntPtr* const* const ptr5 = nullptr; // UndCC_Violation
    std::int8_t* ptr6[10];               // UndCC_Valid
    std::int8_t** ptr7[10];              // UndCC_Valid
}
// Explanation of types
// 1) par1 and ptr1 are of type pointer to std::int8_t.
// 2) par2 and ptr2 are of type pointer to pointer to std::int8_t.
// 3) par3 and ptr3 are of type pointer to a pointer to a pointer
// to std::int8_t.
// This is three levels and is non-compliant.
// 4) par4 and ptr4 are expanded to a type of pointer to a pointer to
// std::int8_t.
// 5) par5 and ptr5 are expanded to a type of const pointer to a const
// pointer
// to a pointer to std::int8_t. This is three levels and is non-compliant.
// 6) par6 is of type pointer to pointer to std::int8_t because arrays
// are converted
// to a pointer to the initial element of the array.
// 7) ptr6 is of type pointer to array of std::int8_t.
// 8) par7 is of type pointer to pointer to pointer to
// std::int8_t because arrays are
// converted to a pointer to the initial element of the array. This is
// three
// levels and is non-compliant.
// 9) ptr7 is of type array of pointer to pointer to std::int8_t. This
// is compliant.
