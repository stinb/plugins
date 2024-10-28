// SEI CERT C++

#include "file.h"
#include <iostream>

extern Car c;
int numWheels = c.get_num_wheels(); // UndCC_Violation

int main() {
  std::cout << numWheels << std::endl;
}
