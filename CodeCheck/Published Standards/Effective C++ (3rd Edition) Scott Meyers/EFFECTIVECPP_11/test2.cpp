// Test cases for format #2:
//  xxx operator=(xxx C &p) { (*this != p) [{]...[}] [return;] }

// Okay.
class Okay2A {
public:
  int a;
  bool operator!=(Okay2A&) {return true;}
  void operator=(Okay2A &rhs)
  {
    if (*this != rhs) {
      a = rhs.a;
    }
  }
};


// Okay.
class Okay2B {
public:
  int a;
  bool operator!=(const Okay2B&) {return true;}
  Okay2B &operator=(const Okay2B &rhs)
  {
    if (*this != rhs) {
      a = rhs.a;
    }
    return *this;
  }
};


// Violation: code before test
class Violation2A {
public:
  int a;
  bool operator!=(Violation2A&) {return true;}
  void operator=(Violation2A &rhs)
  {
    a; // does nothing, but is a violation // UndCC_Violation
    if (*this != rhs)
      a = rhs.a;
  }
};


// Violation: code after test
class Violation2B {
public:
  int a;
  bool operator!=(Violation2B&) {return true;}
  void operator=(Violation2B &rhs)
  {
    if (*this != rhs)
      a = rhs.a;
    a; // does nothing, but is a violation // UndCC_Violation
  }
};


// Violation: test is wrong
class Violation2C {
public:
  int a;
  bool operator!=(const int) {return true;}
  void operator=(Violation2C &rhs)
  {
    if (*this != 0) // UndCC_Violation
      a = rhs.a;
  }
};


// Violation: test is wrong
class Violation2D {
public:
  int a;
  bool operator==(Violation2D&) {return true;}
  void operator=(Violation2D &rhs)
  {
    if (*this == rhs)
      a = rhs.a; // UndCC_Violation
  }
};
