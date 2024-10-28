#include <iostream>
int main() {
  int a = 0; /* UNDCC_Violation */
  int b = 1; /* UNDCC_Valid */
  
  std::cout << b << std::endl;
  std::cout << a << std::endl;
}
