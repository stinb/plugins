// Test cases for format #8:
//  xxx operator=(xxx C &p) { if (&p == this) [{] return...

// Okay.
class Okay8A {
public:
  int a;
  bool operator==(Okay8A&) {return true;}
  void operator=(Okay8A &rhs)
  {
    if (&rhs == this)
      return;
    a = rhs.a;
  }
};


// Okay.
class Okay8B {
public:
  int a;
  bool operator==(Okay8B&) {return true;}
  void operator=(Okay8B &rhs)
  {
    if (&rhs == this) {
      return;
    }
    a = rhs.a;
  }
};


// Violation: member assignment before the test.
class Violation8A {
public:
  int a;
  bool operator==(Violation8A&) {return true;}
  void operator=(Violation8A &rhs)
  {
    a = rhs.a; // UndCC_Violation
    if (&rhs == this)
      return;
  }
};


// Violation: test is incorrect.
class Violation8B {
public:
  int a;
  bool operator!=(Violation8B&) {return true;}
  void operator=(Violation8B &rhs)
  {
    if (&rhs != this)
      return;
    a = rhs.a; // UndCC_Violation
  }
};
