#include <iostream>
#include <new>

int main () {
  char* p = new (std::nothrow) char [1048576]; // UndCC_Valid
  char* p1 = new char [1048576]; // UndCC_Violation
}
