int main()
{
  int a = 4; // UndCC_Valid
  int b;
  if (a > 2) {
    b = 2; // UndCC_Valid
    int c = 5; // UndCC_Violation
    int d; // UndCC_Violation
  }
  return 0;
}
