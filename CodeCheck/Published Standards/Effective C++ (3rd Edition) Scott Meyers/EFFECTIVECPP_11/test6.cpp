// Test cases for format #6:
//  xxx operator=(xxx C &p) { if (*this == p) [{] return...

// Okay.
class Okay6A {
public:
  int a;
  bool operator==(Okay6A&) {return true;}
  void operator=(Okay6A &rhs)
  {
    if (*this == rhs)
      return;
    a = rhs.a;
  }
};


// Okay.
class Okay6B {
public:
  int a;
  bool operator==(Okay6B&) {return true;}
  void operator=(Okay6B &rhs)
  {
    if (*this == rhs) {
      return;
    }
    a = rhs.a;
  }
};


// Violation: member assignment before the test.
class Violation6A {
public:
  int a;
  bool operator==(Violation6A&) {return true;}
  void operator=(Violation6A &rhs)
  {
    a = rhs.a; // UndCC_Violation
    if (*this == rhs)
      return;
  }
};


// Violation: test is incorrect.
class Violation6B {
public:
  int a;
  bool operator!=(Violation6B&) {return true;}
  void operator=(Violation6B &rhs)
  {
    if (*this != rhs)
      return;
    a = rhs.a; // UndCC_Violation
  }
};
