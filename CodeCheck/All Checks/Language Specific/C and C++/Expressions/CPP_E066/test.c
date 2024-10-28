int main() {
  const int max = 10;
  int current = 5;

  if (current = max) { // UndCC_Violation - compiler does not throw error

  }

  if (max == current) { // UndCC_Valid

  }

  // if (max = current) { // UndCC_Valid - compiler throws error

  // }


  while (current == max) { // UndCC_Violation
    ++current;
  }

  int a = (current = max) ? 3 : 4;  // UndCC_Violation

}
