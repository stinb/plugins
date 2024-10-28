#include <iostream>
#include <fstream>

int main() {
    
    // Open a file named "example.txt" for writing
    std::ofstream outputFile("example.txt");

    // Check if the file is opened successfully
    if (!outputFile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1; // Return error code
    }

    // Write some data to the file
    outputFile << "Hello, world!\n";
    outputFile << "This is a sample text file.\n";

    // Close the file
    outputFile.close();

    std::cout << "Data has been written to the file." << std::endl;
    
    std::ofstream file;
    file.open("filename");
    std::string message = "Hello";
    file << message;  // UndCC_Valid - ofstream write

    return 0;
}
