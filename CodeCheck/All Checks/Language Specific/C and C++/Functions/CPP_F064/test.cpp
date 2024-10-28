class Test {
  int value;
  int num;
  
public:
  Test(int v = 0) { value = v; }
  
  int getValue() const { 
    return value;
  }
  int setValue(int v) {
    value = v;
  }
  int getNum() { // UndCC_Violation
    return num;
  }
};
