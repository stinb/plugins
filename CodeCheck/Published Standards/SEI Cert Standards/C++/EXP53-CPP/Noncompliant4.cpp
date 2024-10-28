#include <iostream>

void f()
{
  int *i = new int();   // zero-initializes *i
  int *j = new int{};   // zero-initializes *j
  int *k = new int;     // uninitialized
  int *l = new int{12}; // initializes *l to 12
  std::cout << i << ", " << *i; // UndCC_Valid
  std::cout << j << ", " << *j; // UndCC_Valid
  std::cout << k << ", " << *k; // UndCC_Violation
  std::cout << l << ", " << *l; // UndCC_Valid
}
