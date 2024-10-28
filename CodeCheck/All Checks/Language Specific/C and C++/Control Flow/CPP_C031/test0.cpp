// MISRA C++ 2008

#include <cstdint>
int16_t int16;

void f()
{
  switch ( int16 )
  {
  case 0:
    break;
  case 1:
  case 2:
    break;

  } // UndCC_Violation - default clause is required
  enum Colours { RED, BLUE, GREEN } colour;
  switch ( colour )
  {
    case RED:
      break;
    case GREEN:
      break;

  } // UndCC_Violation - default clause is required
  switch ( colour )
  {
    case RED:
      break;
    case BLUE:
      break;
    case GREEN:
      break;

  } // UndCC_Violation(strict)
}
