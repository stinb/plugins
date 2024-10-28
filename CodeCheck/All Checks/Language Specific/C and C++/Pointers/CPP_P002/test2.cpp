// Test: Pointer must be initialized to 0, or some valid
// value when created.

void func2() {
  int *okay2A=0;
  int *violation2A=NULL; // UndCC_Violation
}
