// Test cases for format #3:
//  xxx operator=(xxx C &p) { (p != *this) [{]...[}] [return;] }

// Okay.
class Okay3A {
public:
  int a;
  bool operator!=(Okay3A&) {return true;}
  void operator=(Okay3A &rhs)
  {
    if (rhs != *this) {
      a = rhs.a;
    }
  }
};


// Okay.
class Okay3B {
public:
  int a;
  bool operator!=(Okay3B&) {return true;}
  Okay3B &operator=(Okay3B &rhs)
  {
    if (rhs != *this) {
      a = rhs.a;
    }
    return *this;
  }
};


// Violation: code before test
class Violation3A {
public:
  int a;
  bool operator!=(Violation3A&) {return true;}
  void operator=(Violation3A &rhs)
  {
    a; // does nothing, but is a violation // UndCC_Violation
    if (rhs != *this)
      a = rhs.a;
  }
};


// Violation: code after test
class Violation3B {
public:
  int a;
  bool operator!=(Violation3B&) {return true;}
  void operator=(Violation3B &rhs)
  {
    if (rhs != *this)
      a = rhs.a;
    a; // does nothing, but is a violation // UndCC_Violation
  }
};


// Violation: test is wrong
class Violation3C {
public:
  int a;
  bool operator!=(Violation3C&) {return true;}
  void operator=(Violation3C &rhs)
  {
    if (rhs != rhs) // UndCC_Violation
      a = rhs.a;
  }
};


// Violation: test is wrong
class Violation3D {
public:
  int a;
  bool operator==(Violation3D&) {return true;}
  void operator=(Violation3D &rhs)
  {
    if (rhs == *this)
      a = rhs.a; // UndCC_Violation
  }
};
