// Custom

#include <string>

void f ( std::string a, std::string &b )
{
  a = ""; // UndCC_Violation
  b = ""; // UndCC_Valid
}
