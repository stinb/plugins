// Test: check for inline functions defined in the class body.

class class1 {
  void okay1();
  void okay2();
  void violation1() {};  // UndCC_Violation
};
inline void class1::okay1() {}
void class1::okay2() {}
