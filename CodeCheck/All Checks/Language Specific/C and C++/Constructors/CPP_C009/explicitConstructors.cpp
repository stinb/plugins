// Test: one argument constructors must be declared explicit.

class Okay1 {
  Okay1();                      // OKAY: no args decl
  Okay1(int,int);               // OKAY: 2 args decl
  explicit Okay1(int);          // OKAY: explicit decl
  Okay1(int,int =0);            // OKAY: assumption 1 ignores default args
};

class Okay2 {
  Okay2() {};                   // OKAY: no args defn
  Okay2(int,int) {};            // OKAY: 2 args defn
  explicit Okay2(int) {};       // OKAY: explicit defn
  Okay2(int,int =0) {};         // OKAY: assumption 1 ignores default args
};

class Violation1 {
  Violation1(int) {};           // UndCC_Violation: one arg, not explicit
};

class Possible1 {
  Possible1(int);               // POSSIBLE VIOLATION: one arg, not explict, no defn
};

