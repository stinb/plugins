// MISRA C++ 2023 Rule 6.4.1
// A variable declared in an inner scope shall not hide a variable declared
// in an outer scope.

short i;

// Global hidden by function local
void f1()
{
  int i;       // UndCC_Violation - hides i in global scope
  int z;
  if (i == 3)
  {
    int z;     // UndCC_Violation - hides z before if
  }
}

// Global hidden by function parameter
void f2(char i)  // UndCC_Violation - hides i in global scope
{
}

// Class member hides global; constructor exception
class C
{
  float i;         // UndCC_Violation - hides i in global scope
  float j;
public:
  // Constructor exception: param only used in init list
  C(float j)       // UndCC_Valid - compliant by exception
    : j(j) {}

  // Constructor param used in body — no exception
  C(float j, float k) // UndCC_Violation - j hides C::j
    : j(j)
  {
    j += k;
  }

  // Member function local hides class member
  void f3()
  {
    int j = 0; // UndCC_Violation - hides C::j
  }
};

// Namespace variable hides global
namespace NS1
{
  int i;       // UndCC_Violation - hides i in global scope
  void f4(int j)  // UndCC_Valid - j does not hide C::j (different scope)
  {
    int l = i + j;
  }
}

// Using-declaration makes name visible in scope
namespace NS2
{
  int v;
}

using NS2::v;

void f5()
{
  float v;         // UndCC_Violation - hides NS2::v via using-declaration
}

// Unscoped enum enumerator hidden
enum E
{
  e0, e1, e2
};

namespace
{
  int e1 = 32; // UndCC_Violation - hides e1 enumerator of E
}

// For-loop variable hiding parameter
void f7(int i)  // UndCC_Violation - hides global i
{
  for (int i = 0; i < 9; ++i) {}  // UndCC_Violation - hides param i

  // Sequential for-loops with same var name — compliant
  for (int j = 0; j < i; ++j) {}  // UndCC_Valid
  for (int j = 0; j < i; ++j) {}  // UndCC_Valid - new scope

  // For-loop var, then local var with same name
  for (int k = 0; k < i; ++k) {}  // UndCC_Valid
  int k = i;                       // UndCC_Valid - for-loop k not in scope
  for (int k = 0; k < i; ++k) {}  // UndCC_Violation - hides k above

  // If-block local hides outer
  if (i > 0)
  {
    int k;    // UndCC_Violation - hides k in outer scope
  }
}

// Lambda: only captured variables can be shadowed
void f8()
{
  char z;
  auto L1 = [z]() { return z; };           // UndCC_Valid - no hiding
  auto L2 = [](char z) { return z; };      // UndCC_Valid - outer z not captured
  auto L3 = []() { char z = 'a'; };        // UndCC_Valid - outer z not captured
  auto L4 = [z]() { char z = 'a'; };       // UndCC_Violation - captured z is hidden
}

// Derived class hides base class member
class Base
{
public:
  int x;
  int y;
};

class Derived : public Base {
public:
  int x;       // UndCC_Violation - hides Base::x
  void method()
  {
    int y = 0; // UndCC_Violation - hides Base::y (visible via inheritance)
  }
};
