#include <cstdlib>
#include <new>
#include <exception> // defines std::set_terminate

void custom_new_handler1() { // UndCC_Violation
  // Returns number of bytes freed.
  extern std::size_t reclaim_resources();
  reclaim_resources();
}

void custom_new_handler2() noexcept(false) { // this is valid because there is a branch that throws std::bad_alloc
  // Returns number of bytes freed.
  extern std::size_t reclaim_resources();
  if (0 == reclaim_resources()) {
    throw std::bad_alloc();
  }
}

void custom_new_handler3() { // this is valid because there is a branch that calls exit.
  // Returns number of bytes freed.
  extern std::size_t reclaim_resources();
  if (0 == reclaim_resources()) {
    exit(1);
  }
}

class CustomBadAlloc : std::bad_alloc {};
void custom_new_handler4() { // this is valid because the type CustomBadAlloc derives from bad_alloc
  throw CustomBadAlloc();
}

extern bool someFunc();
void custom_terminate1() { // UndCC_Violation. custom terminate handlers must terminate the program.
  if (someFunc())
    std::exit(1);
  // this is a path that does not call exit before returning.
}

void custom_terminate2() { // valid because it terminates the program by calling std::exit.
  std::exit(1);
}

void custom_unexpected1() { // UndCC_Violation. custom unexpected handlers must either terminate the program or throw an exception. They must not return.
  if (someFunc())
    std::exit(1);
  // this is a path that does not call exit or throw an exception before returning.
}

void custom_unexpected2() { // valid because it throws an exception or terminates the program by calling exit.
  if (someFunc())
    std::exit(1);
  else
    throw "error";
}

int main() {
  std::set_new_handler(custom_new_handler1);
  std::set_new_handler(custom_new_handler2);
  std::set_new_handler(custom_new_handler3);
  std::set_new_handler(custom_new_handler4);
  std::set_terminate(custom_terminate1);
  std::set_terminate(custom_terminate2);
  std::set_unexpected(custom_unexpected1);
  std::set_unexpected(custom_unexpected2);
}
