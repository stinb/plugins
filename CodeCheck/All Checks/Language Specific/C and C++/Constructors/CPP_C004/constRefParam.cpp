// Test: check that the parameter of assignment operator and copy constructor
// is a constant reference.

class Okay1 {
  Okay1(const Okay1&) {};
  Okay1(Okay1) {}; // not a copy constructor
};


class Violation1 {
  Violation1(Violation1& badAgain) {}; // UndCC_Violation
};
