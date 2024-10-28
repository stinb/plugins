#include <iostream>
#include <filesystem>

#if __cplusplus >= 201703L // C++17

namespace fs = std::filesystem;

int main() {
  fs::path pathToShow(".");
  for (auto& entry : fs::directory_iterator(pathToShow)) {  // UndCC_Valid
    std::cout << entry.path() << std::endl;
  }
  return 0;
}

#endif // C++17
