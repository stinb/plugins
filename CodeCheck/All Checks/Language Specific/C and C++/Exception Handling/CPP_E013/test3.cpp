class class3 {
 public:
  class3();
};

void func4() {
  throw class3(); // Constructor call is okay // UndCC_Violation
}

