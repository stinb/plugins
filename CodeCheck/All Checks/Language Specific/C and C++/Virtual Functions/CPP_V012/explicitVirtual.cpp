// Virtual function not declared explicitly with the virtual keyword.

struct OkayBase {
  virtual int Okay1();
  virtual int Okay2();
  virtual int Okay3() {}
};
int OkayBase::Okay2() {}

struct OkayDerived : OkayBase {
  virtual int Okay1();
  virtual int Okay2();
  virtual int Okay3() {}
};
int OkayDerived::Okay2() {}


struct ViolationBase {
  virtual int Violation1();
  virtual int Violation2();
  virtual int Violation3() {}
};
int ViolationBase::Violation2() {}

class ViolationDerived: public ViolationBase {
  int Violation1(); // UndCC_Violation
  int Violation2(); // UndCC_Violation
  int Violation3() {} // UndCC_Violation
}
int ViolationDerived::Violation2() {}
