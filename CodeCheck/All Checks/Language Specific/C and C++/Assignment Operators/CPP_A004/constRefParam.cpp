// Test: check that the parameter of assignment operator and copy constructor
// is a constant reference.

class Okay1 {
  const Okay1& operator=(const Okay1&) {};
};


class Violation1 {
  const Violation1& operator=(Violation1& bad) {}; // UndCC_Violation
};
