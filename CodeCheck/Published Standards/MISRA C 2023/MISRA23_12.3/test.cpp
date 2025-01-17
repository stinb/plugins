int fn1(int);
int fn2(int, int);


int main() {
  int a = (fn1((1, 2)), fn1((2, 3))); /* UNDCC_Violation Three violation */
  int b = (fn2(1, 2), fn2(2, 3)); /*UNDCC_Violation One violation */
  int c = (1, 2); /* UNDCC_Violation */
  int d = fn2(1, 2); /* UNDCC_Valid */
  int e = 1; /* UNDCC_Valid */
  int f, g, h; // UNDCC_Valid
  int i = 1, j = 2;  // UndCC_Valid
  int k = (1, 2), l = 2; // UNDCC_Violation and FalsePos
  enum m { o = 1, p=2, q=3}; // UNDCC_Valid
  int n[] = {1, 2, 3}; // UNDCC_Valid
}

