#include <iostream>

void riskyFunction() noexcept(false) {}
void riskyFunction2() noexcept(false) {}

int main() {
    riskyFunction();  // UndCC_Violation
  
    try {
        riskyFunction2();  // UndCC_Valid
    } catch (int e) {
        std::cout << "Caught an integer exception: " << e << std::endl;
    }
    
    return 0;
}
