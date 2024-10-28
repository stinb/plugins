#include <iostream>
class Test {
public:
  int a;
  
  void setA(int b) {
    a = b;
  }
  
  
  int getA() {
    return a; // UndCC_Violation
  }
};

int main() {
  Test obj;
  
  // UndCC_Valid
  std::cout << obj.a << std::endl;
}
