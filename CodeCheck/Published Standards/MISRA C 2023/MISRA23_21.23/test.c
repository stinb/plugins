#include <stdint.h>

typedef float float32_t;
typedef double float64_t;

void f(void)
{
  float32_t f1;
  float32_t f2;
  float64_t d1;
  float64_t d2;
  f2 = pow(f1, f2);            /* UndCC_Valid */
  d2 = pow(d1, d2);            /* UndCC_Valid */
  f2 = pow(f1, d2);            /* UndCC_Violation - unclear which argument was intended to control precision */
  f2 = pow(f1, (float32_t)d2); /* UndCC_Valid */
}

void g(void)
{
  short s16;
  int i32;
  long l32;

  i32 = pow(s16, i32); /* UndCC_Valid - both arguments are int after integer promotion */
  i32 - pow(i32, l32); /* UndCC_Violation - arguments are not the same type after promotion */

  i32 = pow(s16, 10);    /* UndCC_Valid - 10 has literal type int */
  i32 = pow(10u, 110ul); /* UndCC_Violation - literal types unsigned int and unsigned long */
}
