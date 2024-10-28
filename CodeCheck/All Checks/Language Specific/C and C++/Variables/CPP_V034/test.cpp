#include <vector>
int main() {
  int a[4]; // UndCC_Violation
  std::vector<int> b; // UndCC_Valid
  
  return 0;
}
