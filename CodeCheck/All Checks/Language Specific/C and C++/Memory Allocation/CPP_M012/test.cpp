#include <iostream>

class Foo {
  public:
    ~Foo();
    // ...
};

class Bar {
  public:
    Foo *f; // flag violation, pointer 'f' is not explicitly deleted in the destructor
    Foo *p;
    ~Bar() { delete p; } // UndCC_Violation
};

int main()
{
  Bar b; 
  b.p = new Foo;

  return 1;
}
