int main() {
  int value;
  int array[5];
  int index = 10;
  value = array[10]; // UndCC_Violation
  value = array[index]; // UndCC_Violation

  return 0;
}
