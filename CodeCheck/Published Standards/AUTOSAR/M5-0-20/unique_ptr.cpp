#include <iostream>
#include <memory>
#include <vector>

// Example class
class MyClass {
public:
    MyClass(int value) : data(value) {}
    void print() const {
        std::cout << "Data: " << data << std::endl;
    }

private:
    int data;
};

int main() {
    // Declare a vector of unique_ptrs to MyClass objects
    std::vector<std::unique_ptr<MyClass>> vec;  // UndCC_Valid - vector declaration

    // Add objects to the vector
    vec.push_back(std::make_unique<MyClass>(10));
    vec.push_back(std::make_unique<MyClass>(20));
    vec.push_back(std::make_unique<MyClass>(30));

    // Access and manipulate objects in the vector
    for (const auto& ptr : vec) {
        ptr->print();
    }

    // Since unique_ptr owns the memory, you don't need to manually delete the objects

    return 0;
}
