#include <vector>
#include <string>

std::vector<std::string> make(std::string);

void bar(std::string s)
{
  for (auto e : make(s)) // UndCC_FalseNeg - call to 'make' and an
  {                      // implicit conversion to std::string_view
  }
  auto r = make(s);
  for (auto e : r) // UndCC_FalseNeg version of the above
  {
  }
}

