#include <cstdint>

class A
{
};
void f1()
{
    void f2(); // UndCC_Violation - function declaration at block scope
    A a1();    // UndCC_Violation - appears to declare an object with no
    // arguments to constructor, but it declares a function
    // 'a1' returning type 'A' and taking no parameters.
    A a2; // Compliant
}
int32_t j;
void f3()
{
    int32_t(j); // UndCC_Violation(1) - declares 'j' (using redundant parentheses)
    int32_t{j}; // UndCC_Valid with this rule, but violates "See also"
} // - Creates a temporary object with value 'j'.

void f(double my_dbl)
{
    int i(int(my_dbl)); // UndCC_Violation, function i declared at block scope, parameter my_dbl initialized by 'most-vexing parse'
}
