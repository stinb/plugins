// Custom

#include <cstddef>
#include <vector>
#include <exception>

class Exception1 : public std::exception {};
class Exception2 : public std::exception {};

void f(std::vector<int> &v, size_t s) noexcept { // UndCC_Violation - exception thrown despite noexcept specification
  throw Exception1{};
}

void g(std::vector<int> &v, size_t s) noexcept(false) { // UndCC_Valid - exceptions are allowed
  throw Exception1{};
}

void thrower()
{
  throw Exception1{};
}

void thrower_helper()
{
  thrower();
}

void h(std::vector<int> &v, size_t s) noexcept(true) {  // UndCC_Violation - exception thrown despite noexcept specification
  thrower_helper();
}

void i(std::vector<int> &v, size_t s) noexcept {  // UndCC_Violation - exception thrown despite noexcept specification
  thrower_helper();
}

void j(std::vector<int> &v, size_t s) noexcept(false) {
  thrower_helper(); // UndCC_Valid - exceptions are allowed
}

void k(std::vector<int> &v, size_t s) throw (Exception2) {
  thrower_helper(); // UndCC_Violation - Exception1 is indirectly thrown
}
