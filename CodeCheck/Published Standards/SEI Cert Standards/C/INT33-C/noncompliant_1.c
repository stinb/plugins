
void test1(int z) {
  if (z == 0) {
    int x = 1 / z; // UndCC_Violation
  }
}
