// Custom

// Classes used for exception handling
class B {};
class D : public B {};
class F : public D, public B {};

void f()
{
  try {
    // ...
  }
  catch (D &d) {
    // ...
  }
  catch (F &f) { // UndCC_Violation
    // ...
  }
}
