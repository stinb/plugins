// SEI CERT C++

class B1 {
  int val;

public:
  B1(int val) : val(val) {}
};

class B2 {
  int otherVal;

public:
  B2(int otherVal) : otherVal(otherVal) {}
};

class D : B1, B2 {
public:
  D(int a) : B1(a), B2(a) {}
};
