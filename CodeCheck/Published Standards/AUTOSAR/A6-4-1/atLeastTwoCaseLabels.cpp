// $Id: A6-4-1.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <cstdint>
void F1(std::uint8_t choice) noexcept
{
  switch (choice)
  { // UndCC_Violation
    default:
      break;
  } 
}
void F2(std::uint8_t choice) noexcept
{
  switch (choice)
  { // UndCC_Violation
    case 0:
      // ...
      break;

    default:
      // ...
    break;
  } 

  if (choice == 0) // UndCC_Valid, an equivalent if statement
  {
    // ...
  }
  else
  {
    // ...
  }

  // ...
  switch (choice)
  {
  case 0:
    // ...
    break;

  case 1:
    // ...
    break;

  default:
    // ...
    break;
  } // Comapliant
}
