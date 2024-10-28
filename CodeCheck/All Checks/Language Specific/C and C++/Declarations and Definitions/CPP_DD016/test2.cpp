// SEI CERT C++

#include <iostream>

struct Widget {
  Widget() { std::cout << "Constructed" << std::endl; }
};

void f() {
  Widget w(); // UndCC_Violation
}
