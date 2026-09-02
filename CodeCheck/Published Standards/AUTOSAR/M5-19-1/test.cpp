// AUTOSAR M5-19-1

#include <cstdint>

struct S
{
  std::uint32_t b : 4;
  std::uint32_t c : 8;
  std::int32_t  d : 4;
  std::uint32_t e : 32;
};

void f(S &s, std::uint32_t v)
{
  std::uint32_t a = 4294967295U + 1U;  // UndCC_Violation - the base rule

  s.b = 15U;                       // UndCC_Valid - fits in 4 bits
  s.b = 16U;                       // UndCC_Violation - wraps a 4-bit field
  s.b = 15U + 1U;                  // UndCC_Violation - constant expression wrapping the field
  s.c = 255U;                      // UndCC_Valid - fits in 8 bits
  s.c = 255U + 1U;                 // UndCC_Violation - wraps an 8-bit field
  s.d = 7;                         // UndCC_Valid - signed bit-field, not this rule
  s.e = 4294967295U;               // UndCC_Valid - the field is the full width of its type
  s.b = v;                         // UndCC_Valid - not a constant

  (void)a;
}

void g(void)
{
  S ok = {15U, 255U, 7, 0U};       // UndCC_Valid
  S bad = {16U, 0U, 0, 0U};        // UndCC_Violation - aggregate initializer wraps the field
  (void)ok; (void)bad;
}

struct P
{
  std::uint32_t a : 4;
  std::uint32_t     : 2;   // unnamed padding
  std::uint32_t b : 4;
  std::uint32_t *ptr;
};

struct Nest { P inner; std::uint32_t n : 4; };

void h(P *p, std::uint32_t v)
{
  P pad = {15U, 15U, nullptr};        // UndCC_Valid - padding consumes no initializer
  P over = {15U, 16U, nullptr};       // UndCC_Violation - second value wraps a 4-bit field
  P des = {.a = 16U, .b = 0U};        // UndCC_Violation - designated initializer
  P des2 = {.a = 0U, .b = 16U};       // UndCC_Violation - designated initializer
  P direct{15U, 15U, nullptr};        // UndCC_Valid - braced direct-init
  Nest nest = {{15U, 15U, nullptr}, 16U};  // UndCC_Violation - nested struct

  p->a = 16U;                         // UndCC_Violation - through a pointer
  p->a = static_cast<std::uint32_t>(16U);  // UndCC_Violation - cast constant
  p->a += 15U;                        // UndCC_Valid - stored value is not constant
  p->a = v;                           // UndCC_Valid - not a constant
  p->a = static_cast<std::uint32_t>(-1);   // UndCC_Violation - converts to 4294967295

  (void)pad; (void)over; (void)des; (void)des2; (void)direct; (void)nest;
}
