#include "test00-m.cpp"

void f02() {
  int x = std::rand() % 10000; // UNDCC_Violation
}
