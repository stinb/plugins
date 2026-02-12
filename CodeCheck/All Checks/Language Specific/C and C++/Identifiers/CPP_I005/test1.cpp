// Custom

struct StructA {
  operator bool()      // UndCC_Valid - not an identifier
  {
  }
};

struct StructB {
  operator bool()      // UndCC_Valid - not an identifier
  {
  }
};

struct Overload {
  void overload(int)   // UndCC_Violation(default)
  {}

  void overload(float) // UndCC_Violation(default)
  {}
};
