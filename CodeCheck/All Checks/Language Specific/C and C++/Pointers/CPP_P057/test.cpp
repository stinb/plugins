#include <memory>

void test() {
  
  int *ptr1 = new int[10]; // UndCC_Violation
  
  // UndCC_Valid
  std::auto_ptr<int> ptr2(new int[10]);
}
