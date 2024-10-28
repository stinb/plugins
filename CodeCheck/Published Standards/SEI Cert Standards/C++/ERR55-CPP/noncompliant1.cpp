// SEI CERT C++

#include <cstddef>
#include <vector>
#include <exception>

class Exception1 : public std::exception {};
class Exception2 : public std::exception {};

void f(std::vector<int> &v, size_t s) noexcept(true) { // UndCC_Violation - exception thrown despite noexcept specification
  throw Exception1{};
}
