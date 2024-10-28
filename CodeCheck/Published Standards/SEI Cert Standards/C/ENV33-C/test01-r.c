#include <stdlib.h>

int not_system(const char *command) {
  return 0;
}

int main() {
  int (*also_system)(const char *) = *&system;
  int (*_also_also_system)(const char *) = also_system;


  system("echo hi");      // UndCC_Violation
  _also_also_system("echo hi"); // UndCC_Violation
  not_system("echo hi");  // UndCC_Valid

  return 0;
}
