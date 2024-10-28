// SEI CERT C++

#include <iostream>

struct Widget {
  Widget() { std::cout << "Constructed" << std::endl; }
};

void f() {
  Widget w1; // Elide the parentheses
  Widget w2{}; // Use direct initialization
}
