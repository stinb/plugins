// Test: destructors in base classes must be virtual.

// OKAY: empty class.
class Okay1 {};

// OKAY: no derived classes, no virtual members
class Okay2 {
  ~Okay2();
  void mem1();
};

// OKAY: virtual, but no derived classes.
class Okay3 {
  virtual ~Okay3();
  void mem1();
};

// OKAY: virtual, with derived class.
class Okay4 {
  virtual ~Okay4();
};

// OKAY: implicit destructor is also virtual.
class Okay4A: public Okay4 {
};
// OKAY: destructor is still virtual.
class Okay4B: public Okay4 {
  ~Okay4B();
};


class Violation1 {}; // UndCC_Violation

// OKAY: no derived classes.
class Okay_Violation1A: public Violation1 {
};


class Violation2 { // UndCC_Violation
  virtual void mem1();
};


class Violation3 {}; // UndCC_Violation


class Violation3A: public Violation3 {}; // UndCC_Violation

// OKAY: no derived classes
class Okay_Violation3B: public Violation3A {};
