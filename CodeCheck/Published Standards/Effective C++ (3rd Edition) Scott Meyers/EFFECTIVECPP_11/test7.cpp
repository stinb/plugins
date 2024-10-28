// Test cases for format #7:
//  xxx operator=(xxx C &p) { if (p == *this) [{] return...

// Okay.
class Okay7A {
public:
  int a;
  bool operator==(Okay7A&) {return true;}
  void operator=(Okay7A &rhs)
  {
    if (rhs == *this)
      return;
    a = rhs.a;
  }
};


// Okay.
class Okay7B {
public:
  int a;
  bool operator==(Okay7B&) {return true;}
  void operator=(Okay7B &rhs)
  {
    if (rhs == *this) {
      return;
    }
    a = rhs.a;
  }
};


// Violation: member assignment before the test.
class Violation7A {
public:
  int a;
  bool operator==(Violation7A&) {return true;}
  void operator=(Violation7A &rhs)
  {
    a = rhs.a; // UndCC_Violation
    if (rhs == *this)
      return;
  }
};


// Violation: test is incorrect.
class Violation7B {
public:
  int a;
  bool operator!=(Violation7B&) {return true;}
  void operator=(Violation7B &rhs)
  {
    if (rhs != *this)
      return;
    a = rhs.a; // UndCC_Violation
  }
};
