#include <iostream>

struct ResourceHolder {
  int* data;

  ResourceHolder() : data(new int[100]) {  // UndCC_Violation, ResourceHolder destructor won't be called, causing memory leak
    std::cout << "Resource acquired." << std::endl;
  }

  ~ResourceHolder() {
    delete[] data;
    std::cout << "Resource released." << std::endl;
  }

  void doWork() {
    throw std::runtime_error("An error occurred.");
  }
};

int main() {
  try {
    ResourceHolder holder;
    holder.doWork();
  } catch (const std::exception& ex) {
    std::cerr << "Caught an exception: " << ex.what() << std::endl;
  }

  return 0;
}
