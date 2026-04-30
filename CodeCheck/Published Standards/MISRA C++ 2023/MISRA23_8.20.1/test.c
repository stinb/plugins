#include <stdint.h>

extern void f(void);
extern void g(void);
extern void h(void);

/* Examples taken from MISRA C 2012 Rule 12.4 */

#define BASE 65024u

void test_switch(uint32_t x)
{
  switch (x)
  {
    case BASE + 0u:
      f();
      break;
    case BASE + 1u:
      g();
      break;
    case (uint32_t)BASE + (uint32_t)4294967295u: /* UndCC_Violation - wraps in unsigned int */
      h();
      break;
  }
}

#define DELAY 10000u
#define WIDTH 60000u

void fixed_pulse(void)
{
  /* On a 32-bit int system, DELAY + WIDTH = 70000u fits in unsigned int. */
  uint32_t big = 0xFFFFFFFFu + 1u;          /* UndCC_Violation - wraps unsigned int */
  uint32_t off_time = DELAY + WIDTH;        /* UndCC_Valid - 70000 fits in 32-bit unsigned */
  (void)big;
  (void)off_time;
}

/* Compliant: c + 1u accesses an object, not a constant expression */
const uint16_t c = 0xffffu;

void compliant_object_access(void)
{
  uint16_t y = c + 1u;                      /* UndCC_Valid */
  (void)y;
}

/* Ternary unevaluated branch is compliant; evaluated branch is not */
void test_ternary(int b)
{
  uint32_t x = (0u == 0u) ? 0u : (0u - 1u); /* UndCC_Valid - false branch unevaluated */
  uint32_t y = b ? 0u : (0u - 1u);          /* UndCC_Violation - branch may evaluate */
  (void)x;
  (void)y;
}

/* Short-circuit logical AND: RHS unevaluated when LHS is false */
void test_short_circuit_and(void)
{
  int a = (0) && ((0u - 1u) > 0u);          /* UndCC_Valid */
  (void)a;
}

/* Short-circuit logical OR: RHS unevaluated when LHS is true */
void test_short_circuit_or(void)
{
  int a = (1) || ((0u - 1u) > 0u);          /* UndCC_Valid */
  (void)a;
}

/* sizeof operand is unevaluated */
void test_sizeof(void)
{
  unsigned long s = sizeof(0u - 1u);        /* UndCC_Valid */
  (void)s;
}

/* Subtraction wrap */
void test_subtraction(void)
{
  uint32_t a = 0u - 1u;                     /* UndCC_Violation */
  uint32_t b = 5u - 10u;                    /* UndCC_Violation */
  uint32_t c2 = 20u - 10u;                  /* UndCC_Valid */
  (void)a; (void)b; (void)c2;
}

/* Multiplication wrap */
void test_multiplication(void)
{
  uint32_t a = 100000u * 100000u;           /* UndCC_Violation - 10^10 > UINT32_MAX */
  uint32_t b = 100u * 100u;                 /* UndCC_Valid */
  (void)a; (void)b;
}

/* Left shift wrap */
void test_shift(void)
{
  uint32_t a = 1u << 33;                    /* UndCC_Violation - shift >= width */
  uint32_t b = 1u << 31;                    /* UndCC_Valid */
  (void)a; (void)b;
}

/* Unary negation wrap */
void test_negation(void)
{
  uint32_t a = -1u;                         /* UndCC_Violation */
  uint32_t b = -0u;                         /* UndCC_Valid */
  (void)a; (void)b;
}

/* Division and modulo of unsigned operands cannot wrap */
void test_div_mod(void)
{
  uint32_t a = 100u / 3u;                   /* UndCC_Valid */
  uint32_t b = 100u % 7u;                   /* UndCC_Valid */
  (void)a; (void)b;
}

/* Non-constant operands: rule does not apply */
void test_non_constant(uint32_t x, uint32_t y)
{
  uint32_t a = x + y;                       /* UndCC_Valid */
  uint32_t b = x + 100u;                    /* UndCC_Valid */
  (void)a; (void)b;
}

/* Sub-expression that wraps even if outer doesn't */
void test_sub_expression(void)
{
  uint32_t a = 10u - 20u + 15u;             /* UndCC_Violation - (10u - 20u) wraps */
  uint32_t b = 10u + 15u - 20u;             /* UndCC_Valid */
  (void)a; (void)b;
}

/* Cast does not hide the wrapping sub-expression */
void test_cast(void)
{
  uint32_t a = (uint32_t)(0u - 1u);         /* UndCC_Violation */
  (void)a;
}
