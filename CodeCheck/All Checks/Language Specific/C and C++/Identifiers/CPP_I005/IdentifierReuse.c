#include <stdint.h>

void func() {
  struct air_speed
  {
  uint16_t speed; /* knots */ // UndCC_Violation
  } * x;
  struct gnd_speed
  {
  uint16_t speed; /* mph */ // UndCC_Violation
  /* Not Compliant - speed is in different units */
  } * y;
  x->speed = y->speed;
}
