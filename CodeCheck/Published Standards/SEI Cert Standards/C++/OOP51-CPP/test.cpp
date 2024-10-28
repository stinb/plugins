#include <iostream>

class Base {
public:
    int baseValue;
    Base(int val) : baseValue(val) {}
    void print() const {
        std::cout << "Base Value: " << baseValue << std::endl;
    }
};

class Derived : public Base {
public:
    int derivedValue;
    Derived(int baseVal, int derivedVal) : Base(baseVal), derivedValue(derivedVal) {}
    void print() const {
        std::cout << "Base Value: " << baseValue << ", Derived Value: " << derivedValue << std::endl;
    }
};

int main() {
    Derived derivedObj(10, 20);
    Base baseObj = derivedObj; // UndCC_Violation
    baseObj.print(); // Calls the print function of Base class
    return 0;
}
