#include <string>

void f1(std::string &&);      // #1
void f1(std::string const &); // #2
void f2(std::string const &s1, std::string s2)
{
    f1(s1);                            // Calls #2
    f1(std::move(s1));                 // UndCC_Violation - calls #2
    f1(std::move(s2));                 // Compliant - calls #1
    f1(std::string("abc"));            // Calls #1
    f1(std::move(std::string("abc"))); // UndCC_Violation - redundant move of
}
