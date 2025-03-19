// Custom

#include <iostream>

class Dog {
  public:
    static void bark()
    {
      std::cout << "RARRRRR!!!";
    }
};

class Chihuahua: public Dog {
  public:
    static void bark()
    {
      std::cout << "waaa";
    }
};

void dogPark()
{
  void (*p2)(void) = Chihuahua::bark;  // UndCC_Violation
  void (*p3)(void) = &Chihuahua::bark; // UndCC_Valid
  void (*p4)(void) = (Dog::bark);      // UndCC_Violation
  void (*p5)(void) = &(Dog::bark);     // UndCC_Valid

  Dog dog = Dog();                     // UndCC_Valid

  std::cout << "Bark!\n";              // UndCC_Valid
  std::cout << "Bark!" << &std::endl;  // UndCC_Valid
  std::cout << "Bark!" << std::endl;   // UndCC_Violation(strict)
}
