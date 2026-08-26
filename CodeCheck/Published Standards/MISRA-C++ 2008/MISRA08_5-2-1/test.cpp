// MISRA C++ 2008 - 5-2-1

#include <compare>
#include <cstdint>

bool is_odd(std::int32_t);

void f(std::int32_t x, std::int32_t c1, std::int32_t c2, std::int32_t c3,
       bool ishigh, bool y, bool z)
{
  if (x == 0 && ishigh) {}          // UndCC_Violation
  if ((x == 0) && ishigh) {}        // UndCC_Valid
  if (x || y || z) {}               // UndCC_Valid - exception, sequence of only ||
  if (x || y && z) {}               // UndCC_Violation
  if (x || (y && z)) {}             // UndCC_Valid
  if (x && !y) {}                   // UndCC_Violation
  if (x && (!y)) {}                 // UndCC_Valid
  if (is_odd(y) && x) {}            // UndCC_Valid
  if ((x > c1) && (y > c2) && (z > c3)) {}  // UndCC_Valid - exception
  if ((x > c1) && (y > c2) || (z > c3)) {}  // UndCC_Violation
  if ((x > c1) && ((y > c2) || (z > c3))) {}  // UndCC_Valid
}

struct S
{
  bool b;
  bool m();
};

void g(S *s, S &r, S a[2], std::int32_t i, std::int32_t *p, bool y)
{
  if (s->b && r.b) {}               // UndCC_Valid - member access is postfix
  if (a[0].b && s->m()) {}          // UndCC_Valid - subscript and call
  if (i++ && y) {}                  // UndCC_Valid - postfix increment
  if (++i && y) {}                  // UndCC_Violation - prefix increment
  if (*p && y) {}                   // UndCC_Violation - dereference
  if ((*p) && y) {}                 // UndCC_Valid
  if (static_cast<bool>(i) && y) {} // UndCC_Valid - named cast is postfix
  if ((bool)i && y) {}              // UndCC_Violation - cast is not postfix
  if (y && (i > 0 ? r.b : s->b)) {} // UndCC_Valid
  if (sizeof(i) && y) {}            // UndCC_Violation - sizeof is not postfix
}

struct B
{
  operator bool() const;
  bool m() const;
  B operator*() const;
  B &operator++();
  B operator++(std::int32_t);
  B operator[](std::int32_t) const;
  B operator()(std::int32_t) const;
  bool operator!() const;
};

void h(B a, B b, B *p, bool y)
{
  if (a && y) {}        // UndCC_Valid
  if (*a && y) {}       // UndCC_Violation - overloaded dereference
  if ((*a) && y) {}     // UndCC_Valid
  if (a.operator bool() && y) {}  // UndCC_Valid - explicit conversion call
  if (++a && y) {}      // UndCC_Violation - overloaded prefix increment
  if (a++ && y) {}      // UndCC_Valid - overloaded postfix increment
  if (a[0] && y) {}     // UndCC_Valid - overloaded subscript
  if (a(0) && y) {}     // UndCC_Valid - overloaded call
  if (p->m() && y) {}   // UndCC_Valid - member call
  if (!a && y) {}       // UndCC_Violation - overloaded negation
  if (a && b && y) {}   // UndCC_Valid - exception, sequence of only &&
  if (noexcept(y) && y) {}  // UndCC_Violation - noexcept is not postfix
}

struct C
{
  auto operator<=>(const C &) const = default;
  bool operator==(const C &) const = default;
};

void k(C a, C c, bool y)
{
  if (a < c && y) {}    // UndCC_Violation - rewritten comparison
  if ((a < c) && y) {}  // UndCC_Valid
}
