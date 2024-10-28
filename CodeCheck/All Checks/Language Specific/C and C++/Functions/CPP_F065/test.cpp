class A {
public:
  // UndCC_Valid
  int a1(const int b){
    return b;
  }
  
  int a2(int c) {  // UndCC_Violation
    return c;
  }
  // UndCC_Valid
  int a3(int d) {
    return ++d;
  }
};


