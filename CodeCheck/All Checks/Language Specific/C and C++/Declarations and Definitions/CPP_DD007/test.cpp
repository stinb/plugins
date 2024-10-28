#include <iostream>
#include <string>
using namespace std;

class Person {  // UndCC_Valid
  public:
    int age;

  private:
    int id;

};

class Vehicle { // UndCC_Violation
  public:
    string make;
    string model;

  protected:
    int mileage;


};

int main() {

  return 0;
}
