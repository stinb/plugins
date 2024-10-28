/* MISRA C 2012 */

#include <stdint.h>

int16_t errorflag, next;

void f()
{
  int16_t x;

  switch ( x )
  {
    case 0:
      ++x;
      break;
    case 1:
    case 2:
      break;
  }                   /* UndCC_Violation - default label is Required    */
}

void g()
{
  int16_t x;

  switch ( x )
  {
    case 0:
      ++x;
      break;
    case 1:
    case 2:
      break;
    default:          /* Compliant - default label is present           */
      errorflag = 1;  /* should be non-empty if possible                */
      break;
  }                   /* UndCC_Valid */
}

void h()
{
  enum Colours
  { RED, GREEN, BLUE } colour;

  switch ( colour )
  {
    case RED:
      next = GREEN;
      break;
    case GREEN:
      next = BLUE;
      break;
    case BLUE:
      next = RED;
      break;
  }                   /* UndCC_Violation(strict)  */
}
