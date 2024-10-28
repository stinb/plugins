class Test {
private:
  int a, b;
public:
  Test operator+ (Test const &obj) {   // UndCC_Valid
    Test t;
    t.a = a + obj.a;
    t.b = b + obj.b;
    return t;
  }


  Test operator- (Test const &obj) {  // UndCC_Violation
    Test t;
    t.a = a * obj.a;
    return t;
  }
};
