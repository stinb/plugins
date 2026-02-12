// MISRA C 2004

#include <stdint.h>

void test()
{
   struct air_speed
   {
      uint16_t speed;   /* UndCC_Violation - knots */
   } * x;
   struct gnd_speed
   {
      uint16_t speed;   /* UndCC_Violation - mph */
                        /* Not Compliant - speed is in different units */
   } * y;
   x->speed = y->speed;
}
