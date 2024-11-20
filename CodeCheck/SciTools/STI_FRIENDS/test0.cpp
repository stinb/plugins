class Outer;


class Inner {
public:
  Inner(int value): mValue(value) {}

private:
  int mValue;
  friend class Outer; // UndCC_Valid
};


class Outer {
public:
  Outer(int value): mValue(value) {}
  Outer(const Inner &inner): mValue(inner.mValue) {}

private:
  int mValue;
  friend class Inner; // UndCC_Violation
};
