// Test code for destructorsNotThrowExceptions.pl.

void nothrows1() throw () {}
void throws1() throw (int) {}
void anythrows1() {}

// Okay: catch ...
class Okay1A {
  ~Okay1A() {
    try {
      nothrows1();
      throws1();
      anythrows1();
    } catch (...) {};
  }
};

// Okay: catch doesn't match throw, but we're not enforcing that.
class Okay1B {
  ~Okay1B() {
    try {
      throws1();
    } catch (float) {};
  }
};

// Okay: called functions are defined as no throw.
class Okay1B {
  ~Okay1B() {
    nothrows1();
  }
};

// Okay: nested try blocks.
class Okay1C {
  ~Okay1C() {
    try {
      throws1();
      try {
        throws1();
      } catch (...) {};
      throws1();
    } catch (...) {};
  }
};



class Violation1A {
  ~Violation1A() {
    throws1(); // UndCC_Violation
  }
};


class Violation1B {
  ~Violation1B() {
    anythrows1(); // UndCC_Violation
  }
};


class Violation1C {
  ~Violation1C() {
    try {
      throws1();
    } catch (...) {};
    throws1(); // UndCC_Violation
  }
};


class Violation1D {
  ~Violation1D() {
    try {
      try {
      } catch (...) {};
    } catch (...) {};
    throws1(); // UndCC_Violation
  }
};
