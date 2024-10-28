#include <iostream>

void unusedParameterFunction(int unusedParameter) {  // UndCC_Violation
  // The parameter is not used or changed in any way
  std::cout << "This function does not use the parameter." << std::endl;
}

int main() {
  unusedParameterFunction(42);
  return 0;
}
