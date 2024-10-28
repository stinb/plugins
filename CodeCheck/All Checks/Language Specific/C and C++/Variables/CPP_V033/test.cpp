#include <iostream>
#include "test1.h"

using namespace std;
typedef unsigned int uint;

class MyClass {
public:
  int myNum;
  ulong myUlong;
  ulong myFun(ulong a) {
    return a;
  }
};

class MyClass1 {
public:
  int myNum;
  uint myUint; // UndCC_Violation
  uint myFun(uint a) { // UndCC_Violation
    return a;
  }
};

int main() {
  return 0;
}
