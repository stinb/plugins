struct S1 {};
struct S2 {};

struct Okay1 {
  int a,b;
  int c;
  Okay1();
  Okay1(int) : a(0), b(0), c(0) {}
};

struct Okay2: Okay1 {
  int d,e;
  Okay2() : Okay1(), d(0), e(0) {}
};

struct Okay3: S1, S2 {
  Okay3() : S1(), S2() {}
};



struct Violation1 {
  int a,b;
  int c;
  Violation1() : b(0), a(0), c(0) {}; // UndCC_Violation
};

struct Violation2: Violation1 {
  int d,e;
  Violation2() : d(0), Violation1(), e(0) {}; // UndCC_Violation
};

struct Violation3: S1, S2 {
  Violation3() : S2(), S1() {}; // UndCC_Violation
};
