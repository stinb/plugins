class BaseClass {
public:
  virtual void disp(){} // UndCC_Valid
private:
  virtual void p() {} // UndCC_Violation
protected:
  virtual void o() {}
};
class DerivedClass: public BaseClass{
public:
  void disp() {} // UndCC_Valid
  void p() {}  // UndCC_Valid
};
