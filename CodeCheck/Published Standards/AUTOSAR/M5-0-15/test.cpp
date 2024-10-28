#include <stdint.h>
#include <inttypes.h>

void my_fn(uint8_t * p1, uint8_t p2[])
{
  uint8_t index = 0U;
  uint8_t * p3;
  uint8_t * p4;

  *p1 = 0U;
  p1 ++;
  p1 = p1 + 5;     /* UndCC_Violation - pointer increment */
  p1[5] = 0U;
  p3 = &p1[5];
  p2[0] = 0U;
  index ++;
  index = index + 5U;
  p2[index] = 0U;  /* UndCC_Valid */
  p4 = &p2[5];     /* UndCC_Valid */
}

