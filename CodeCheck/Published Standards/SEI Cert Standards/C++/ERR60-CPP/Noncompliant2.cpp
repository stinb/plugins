#include <iostream>
#include <stdexcept>

class CustomException {
public:
    CustomException() {}

    // Copy constructor that may throw an exception
    CustomException(const CustomException& other) {
        if (shouldThrow()) {
            throw std::runtime_error("Failed to copy CustomException");
        }
    }

private:
    bool shouldThrow() const {
        return true; // Simulating a condition where copying fails
    }
};

int main() {
    try {
        throw CustomException();
    } catch (const CustomException& ex) {       // UndCC_Violation
        std::cerr << "Caught an exception." << std::endl;
    }

    return 0;
}
