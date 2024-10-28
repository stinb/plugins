// SEI CERT C++

#include "file.h"
#include <iostream>

int &get_num_wheels() {
  extern Car c;
  static int numWheels = c.get_num_wheels();
  return numWheels;
}

int main() {
  std::cout << get_num_wheels() << std::endl;
}
