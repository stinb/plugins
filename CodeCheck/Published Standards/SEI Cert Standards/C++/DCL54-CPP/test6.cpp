#include <iostream>
#include <new>

class MyClass {
public:
    // Overloaded allocation function at class scope
    void* operator new(std::size_t size) { // UndCC_Violation
        std::cout << "Custom allocation in MyClass" << std::endl;
        return std::malloc(size);
    }
};

// Overloaded deallocation function at global scope
void operator delete(void* ptr) { // UndCC_Violation
    std::cout << "Custom deallocation at global scope" << std::endl;
    std::free(ptr);
}

int main() {
    MyClass* obj = new MyClass(); // Calls the custom allocation function in MyClass
    delete obj; // Calls the global deallocation function

    return 0;
}
