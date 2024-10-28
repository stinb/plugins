#include <iostream>
using namespace std;

class Animal
{
public:
  Animal() {
    cout << "Animal class constructor invoked" << endl;
  }
  ~Animal() {
    cout << "Animal class destructor invoked" << endl;
  }
};

int main()
{
  Animal* dog = new Animal; // UndCC_Violation

  delete dog;               // UndCC_Violation

  return 0;
}

