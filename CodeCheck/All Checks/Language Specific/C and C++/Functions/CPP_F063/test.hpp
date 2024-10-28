class MyClass {
  public:
  int num;
  int num1;

  int getNum(int num, int num2) { // UndCC_Violation
    return num;
  }
};
