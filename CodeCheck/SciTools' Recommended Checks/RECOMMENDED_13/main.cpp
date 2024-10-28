#include "header.h"
#include <cstdio>
#include <iostream>

int valid () {
	/* called in other file */
	return 42;
}
int violation () { // UndCC_Violation(0)
	/* not called */
	return 0;
}


class otherclass {
  ~otherclass() {} // inline destructor
};

void funcpointer(int x)
{
  printf( "%d\n", x );
}

/*Virtual functions are considered called if one of their overridden functions is */
class Animal {
public:
  virtual void eat() const {
    std::cout << "I eat like a generic Animal." << std::endl;
  }
  virtual ~Animal() {
  }
};

class Wolf : public Animal {
public:
  void eat() const {
    std::cout << "I eat like a wolf!" << std::endl;
  }
};

int main() {
	/* main ignored */
	return valid();
  void (*foo)(int);
  /* the ampersand is actually optional */
  foo = &funcpointer;
  Animal *blob = new Animal();
  Wolf *growl = new Wolf();
  growl->eat();
}
