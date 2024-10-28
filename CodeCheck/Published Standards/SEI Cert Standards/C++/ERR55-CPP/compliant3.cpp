// SEI CERT C++

#include <exception>

class Exception1 : public std::exception {};
class Exception2 : public std::exception {};

void foo() {
  throw Exception2{}; // UndCC_Valid - foo() promises nothing about exceptions
}

void bar() throw (Exception1, Exception2) {
  foo();
}
