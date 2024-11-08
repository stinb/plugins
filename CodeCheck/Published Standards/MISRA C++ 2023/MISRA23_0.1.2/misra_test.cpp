#include <cstdint>
#include <string>
#include <functional>

uint16_t func();
void discarded()
{
    func();          // UndCC_Violation - implicitly discarded
    (void)func();    // UndCC_Valid - void cast is a use
    auto b = func(); // UndCC_Valid - used as initializer
}
void f1(std::string q)
{
    std::string s{q}; // Rule does not apply - not function call syntax
    s = q;            // Rule does not apply - not function call syntax
    s.operator=(q);   // UndCC_Violation
}
void f2(std::function<int()> &f)
{
    f(); // UndCC_FalseNeg - using function call syntax
    auto a = []()
    { return 10; };
    a(); // UndCC_FalseNeg - using function call syntax
}
