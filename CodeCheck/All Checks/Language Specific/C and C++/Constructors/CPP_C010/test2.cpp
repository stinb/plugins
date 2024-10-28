#include <iostream>

class A {
public:
	A(){}
  A(int p) { std::cout << "A called\n"; }
};

class B : public A {
public:
  B() {}
};

int main()
{
  B b;
  return 0;
}
