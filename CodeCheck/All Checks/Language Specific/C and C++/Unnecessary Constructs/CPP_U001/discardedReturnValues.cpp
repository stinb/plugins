// Test for discarded return values

void no_return(int =0) {}
int returns_value() { return 1; }

int Okay1 = returns_value();

void Okay2() {
  no_return();
  int a = returns_value();
  a += returns_value();
  if (returns_value()) ;
  if (returns_value()) {}
  while (returns_value()) {}
  do {} while (returns_value());
  switch (returns_value()) {}
  for (; returns_value(); ) {}
  no_return(a=returns_value());
}

int Okay3() {
  return (returns_value()) + returns_value();
}

void Violation1() {
  returns_value(); // UndCC_Violation
  no_return(),returns_value(); // UndCC_Violation
  for (;;returns_value()) {} // UndCC_Violation
}

void Test1() {
  no_return(returns_value());
}

struct ReturnsValue {
  int returns_value() { return 1; }
};
struct ReturnsObj {
  ReturnsValue *obj();
};
void Test2() {
  ReturnsObj R = new ReturnsObj;
  R->returns_value(); // UndCC_Violation
}
