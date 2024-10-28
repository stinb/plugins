// Custom

void throws_nothing_0() throw ();
void throws_nothing_1() noexcept;
void throws_nothing_2() noexcept(true);
void throws_nothing_3() noexcept(1);
void throws_nothing_4() noexcept(1 + 1);

void possibly_throws();

void throws_int() throw (int);
void throws_something_0() noexcept(false);
void throws_something_1() noexcept(0);
void throws_something_2() noexcept(1 - 1);

// Okay: catch ...
class Okay1A {
  ~Okay1A() {
    try {
      throws_nothing_0();
      throws_nothing_1();
      throws_nothing_2();
      throws_nothing_3();
      throws_nothing_4();
      possibly_throws();
      throws_int();
      throws_something_0();
      throws_something_1();
      throws_something_2();
      throw 1;
    } catch (...) {}
  }
};

// Okay: catch doesn't match throw, but we're not enforcing that
class Okay1B {
  ~Okay1B() {
    try {
      throws_nothing_0();
      throws_nothing_1();
      throws_nothing_2();
      throws_nothing_3();
      throws_nothing_4();
      possibly_throws();
      throws_int();
      throws_something_0();
      throws_something_1();
      throws_something_2();
      throw 1;
    } catch (float) {}
  }
};

// Okay: called functions are defined as no throw
class Okay1C {
  ~Okay1C() {
    throws_nothing_0();
    throws_nothing_1();
    throws_nothing_2();
    throws_nothing_3();
    throws_nothing_4();
  }
};

// Okay: nested try blocks
class Okay1D {
  ~Okay1D() {
    try {
      throws_nothing_0();
      throws_nothing_1();
      throws_nothing_2();
      throws_nothing_3();
      throws_nothing_4();
      possibly_throws();
      throws_int();
      throws_something_0();
      throws_something_1();
      throws_something_2();
      throw 1;
      try {
        throws_nothing_0();
        throws_nothing_1();
        throws_nothing_2();
        throws_nothing_3();
        throws_nothing_4();
        possibly_throws();
        throws_int();
        throws_something_0();
        throws_something_1();
        throws_something_2();
        throw 1;
      } catch (...) {}
      throws_nothing_0();
      throws_nothing_1();
      throws_nothing_2();
      throws_nothing_3();
      throws_nothing_4();
      possibly_throws();
      throws_int();
      throws_something_0();
      throws_something_1();
      throws_something_2();
      throw 1;
    } catch (...) {}
  }
};

class Violation1A {
  ~Violation1A() {
    throws_nothing_0();
    throws_nothing_1();
    throws_nothing_2();
    throws_nothing_3();
    throws_nothing_4();
    possibly_throws();    // UndCC_Valid - assumed by option to not throw
    throws_int();         // UndCC_Violation
    throws_something_0(); // UndCC_Violation
    throws_something_1(); // UndCC_Violation
    throws_something_2(); // UndCC_FalseNeg - complicated noexcept
    throw 1;              // UndCC_Violation
  }
};

class Violation1B {
  ~Violation1B() {
    try {
      throws_nothing_0();
      throws_nothing_1();
      throws_nothing_2();
      throws_nothing_3();
      throws_nothing_4();
      possibly_throws();
      throws_int();
      throws_something_0();
      throws_something_1();
      throws_something_2();
      throw 1;
    } catch (...) {}
    throws_nothing_0();
    throws_nothing_1();
    throws_nothing_2();
    throws_nothing_3();
    throws_nothing_4();
    possibly_throws();    // UndCC_Valid - assumed by option to not throw
    throws_int();         // UndCC_Violation
    throws_something_0(); // UndCC_Violation
    throws_something_1(); // UndCC_Violation
    throws_something_2(); // UndCC_FalseNeg - complicated noexcept
    throw 1;              // UndCC_Violation
  }
};

class Violation1C {
  ~Violation1C() {
    try {
      try {
      } catch (...) {}
    } catch (...) {}
    throws_nothing_0();
    throws_nothing_1();
    throws_nothing_2();
    throws_nothing_3();
    throws_nothing_4();
    possibly_throws();    // UndCC_Valid - assumed by option to not throw
    throws_int();         // UndCC_Violation
    throws_something_0(); // UndCC_Violation
    throws_something_1(); // UndCC_Violation
    throws_something_2(); // UndCC_FalseNeg - complicated noexcept
    throw 1;              // UndCC_Violation
  }
};
