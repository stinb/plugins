// Custom

// Structs used for exception handling
struct B {};
struct D : public B {};
struct F : public D {};

void f()
{
  try {
    // ...
  }
  catch (B &b) {
    // ...
  }
  catch (F &f) { // UndCC_Violation
    // ...
  }
}
