void func(int &max);
int main() {
  int max = 10;
  for(int i = 0; i < max; i++) {
    max = 9;                          // UndCC_Violation
    ++max;                            // UndCC_Violation
    func(max);                        // UndCC_Violation because passed by reference
  }
}

