// MISRA C++ 2008

#include <cstdint>

int fun(int a) {
  return 0;
}
int main() {
  int32_t i;
  int32_t j;
  volatile int32_t k;
  j = sizeof( i = 1234 );     // UndCC_Violation - j is set to the sizeof the
                              // type of i which is an int32_t.
                              // i is not set to 1234.

  j = sizeof ( k );           // UndCC_Valid by exception.

  j = sizeof ( fun(3) );      // UndCC_Violation - Functions could trigger side effects
}
