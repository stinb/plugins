#include <iostream>
using namespace std;

class Car {           /* UndCC_Violation percent lack of cohesion high, class instance variables not used in method */
public:
    int year;
    int numWheels;
    void speed();
};

void Car::speed() {
    // do nothing
}

int main() {
    Car myObj;
    myObj.speed();
    return 0;
}
