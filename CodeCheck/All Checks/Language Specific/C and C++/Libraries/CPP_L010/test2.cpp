// SEI CERT C++

#include <iostream>

struct Counter {
  static int instances;
  Counter() { ++instances; }
  ~Counter() { --instances; }
};

int Counter::instances = 0;

void f() {
  Counter c;
  std::cout << "f(): Instances: " << Counter::instances << std::endl;
  throw "Exception";
}

int main() {
  std::cout << "Before throw: Instances: " << Counter::instances << std::endl;
  try {
    f();
  } catch (const char *E) {
    std::cout << "From catch: Instances: " << Counter::instances << std::endl;
  }
  std::cout << "After catch: Instances: " << Counter::instances << std::endl;
}
