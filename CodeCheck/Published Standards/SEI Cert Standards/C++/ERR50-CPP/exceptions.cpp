
#include <exception>
 
void report(const char *msg) noexcept;
[[noreturn]] void fast_fail(const char *msg) {
  // Report error message to operator
  report(msg);
  
  // Terminate
  std::terminate(); // UndCC_FalsePositive
}
  
void critical_function_that_fails() noexcept(false);
  
void f() {
  try {
    critical_function_that_fails();
  } catch (...) {
    fast_fail("Critical function failure");
  }
}