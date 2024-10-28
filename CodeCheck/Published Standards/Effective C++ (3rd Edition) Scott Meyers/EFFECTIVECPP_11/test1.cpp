// Test cases for format #1:
//  xxx operator=(xxx C &p) { (this != &p) [{]...[}] [return;] }

// Okay.
class Okay1A {
public:
  int a;
  void operator=(Okay1A &rhs)
  {
    if (this != &rhs) {
      a = rhs.a;
    }
  }
};


// Okay.
class Okay1B {
public:
  int a;
  Okay1B &operator=(Okay1B &rhs)
  {
    if (this != &rhs) {
      a = rhs.a;
    }
    return *this;
  }
};


// Violation: no test.
class Violation1A {
public:
  int a;
  void operator=(Violation1A &rhs)
  {
    a = rhs.a; // UndCC_Violation
  }
};


// Violation: code before test
class Violation1B {
public:
  int a;
  void operator=(Violation1B &rhs)
  {
    a; // does nothing, but is a violation // UndCC_Violation
    if (this != &rhs)
      a = rhs.a;
  }
};


// Violation: code after test
class Violation1C {
public:
  int a;
  void operator=(Violation1C &rhs)
  {
    if (this != &rhs)
      a = rhs.a;
    a; // does nothing, but is a violation // UndCC_Violation
  }
};


// Violation: test is wrong
class Violation1D {
public:
  int a;
  void operator=(Violation1D &rhs)
  {
    if (this != 0) // UndCC_Violation
      a = rhs.a;
  }
};


// Violation: test is wrong
class Violation1E {
public:
  int a;
  void operator=(Violation1E &rhs)
  {
    if (this < &rhs) // UndCC_Violation
      a = rhs.a;
  }
};


// Violation: empty statement after {}
class Violation1F {
public:
  int a;
  void operator=(Violation1F &rhs)
  {
    if (this != &rhs)
      a = rhs.a;
    ; // UndCC_Violation
  }
};
