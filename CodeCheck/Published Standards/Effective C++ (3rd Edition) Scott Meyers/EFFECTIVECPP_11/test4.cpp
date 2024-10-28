// Test cases for format #4:
//  xxx operator=(xxx C &p) { (&p != this) [{]...[}] [return;] }

// Okay.
class Okay4A {
public:
  int a;
  void operator=(Okay4A &rhs)
  {
    if (&rhs != this) {
      a = rhs.a;
    }
  }
};


// Okay.
class Okay4B {
public:
  int a;
  Okay4B &operator=(Okay4B &rhs)
  {
    if (&rhs != this) {
      a = rhs.a;
    }
    return *this;
  }
};


// Violation: code before test
class Violation4A {
public:
  int a;
  void operator=(Violation4A &rhs)
  {
    a; // does nothing, but is a violation // UndCC_Violation
    if (&rhs != this)
      a = rhs.a;
  }
};


// Violation: code after test
class Violation4B {
public:
  int a;
  void operator=(Violation4B &rhs)
  {
    if (&rhs != this)
      a = rhs.a;
    a; // does nothing, but is a violation // UndCC_Violation
  }
};


// Violation: test is wrong
class Violation4C {
public:
  int a;
  void operator=(Violation4C &rhs)
  {
    if (&rhs != &rhs) // UndCC_Violation
      a = rhs.a;
  }
};


// Violation: test is wrong
class Violation4D {
public:
  int a;
  void operator=(Violation4D &rhs)
  {
    if (&rhs < this) // UndCC_Violation
      a = rhs.a;
  }
};
