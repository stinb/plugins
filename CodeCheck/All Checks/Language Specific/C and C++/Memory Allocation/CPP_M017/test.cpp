#include <cstdlib>
class A {
public:
  int* c;
  
  void mf() {
    // UndCC_Valid
    int* a = new int;
    delete(a);
    
    
    int* aa = new int[4]; // UndCC_Violation
    
    // UndCC_Valid
    int *b = static_cast<int *>(std::malloc(sizeof(int)));
    std::free(b);
    
    //Non-compliant
    int *bb = static_cast<int *>(std::malloc(sizeof(int))); // UndCC_Violation
  }
};
