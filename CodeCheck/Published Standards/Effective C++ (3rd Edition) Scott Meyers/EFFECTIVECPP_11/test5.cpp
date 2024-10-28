// Test cases for format #5:
//  xxx operator=(xxx C &p) { if (this == &p) [{] return...

// Okay.
class Okay5A {
public:
  int a;
  void operator=(Okay5A &rhs)
  {
    if (this == &rhs)
      return;
    a = rhs.a;
  }
};


// Okay.
class Okay5B {
public:
  int a;
  void operator=(Okay5B &rhs)
  {
    if (this == &rhs) {
      return;
    }
    a = rhs.a;
  }
};


// Violation: member assignment before the test.
class Violation5A {
public:
  int a;
  void operator=(Violation5A &rhs)
  {
    a = rhs.a; // UndCC_Violation
    if (this == &rhs)
      return;
  }
};


// Violation: test is incorrect.
class Violation5B {
public:
  int a;
  void operator=(Violation5B &rhs)
  {
    if (this != &rhs)
      return;
    a = rhs.a; // UndCC_Violation
  }
};
