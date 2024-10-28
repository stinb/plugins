#include <cstdlib>
 
void throwing_func() noexcept(false);
 
void f() { // Not invoked by the program except as an exit handler.
  try {
    throwing_func();
  } catch (...) {
    // Handle error
  }
}
 
int main() {
  if (0 != std::atexit(f)) { // UndCC_FalsePos
    // Handle error
  }
  // ...
}
