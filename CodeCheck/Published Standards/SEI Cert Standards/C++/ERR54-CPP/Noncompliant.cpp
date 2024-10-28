// SEI CERT C++

// Classes used for exception handling
class B {};
class D : public B {};

void f() {
  try {
    // ...
  } catch (B &b) {
    // ...
  } catch (D &d) { // UndCC_Violation
    // ...
  }
}
