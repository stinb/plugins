// MISRA C++ 2008

#include <cstdint>

void infeas ( uint8_t para, uint8_t outp )
{
 // The condition below will always be true hence the path
 // for the false condition is infeasible. Non-compliant.
 if ( para >= 0U ) // UndCC_Violation
 {
  outp = 1U;
 }
 // The following if statement combines with the if
 // statement above to give four paths. One from
 // the first condition is already infeasible and
 // the condition below combined with assignment above
 // makes the false branch infeasible. There is therefore
 // only one feasible path through this code.
 if ( outp == 1U )
 {
  outp = 0U;
 }
}

void fn ( )
{
  uint16_t u16a;
  int8_t s8a;

  enum ec { RED, BLUE, GREEN } col;
  if ( col <= GREEN ) // UndCC_FalseNeg - always true
                      // (actually, col can be 3 or greater by casting)
  {
    // Will always get here
  }
  else
  {
    // Will never get here
  }
  // The following ifs exhibit similar behaviour.
  // Note that u16a is a 16-bit unsigned integer
  // and s8a is an 8-bit signed integer.
  if ( u16a < 0U ) { }                    // UndCC_Violation - u16a is always >= 0
  if ( u16a <= 0xffffU ) { }              // UndCC_Violation - always true
  if ( s8a < 130 ) { }                    // UndCC_Violation - always true
  if ( ( s8a < 10 ) && ( s8a > 20 ) ) { } // UndCC_Violation - always false
  if ( ( s8a < 10 ) || ( s8a > 5 ) ) { }  // UndCC_Violation - always true
  // Nested conditions can also cause problems
  if ( s8a > 10 )
  {
    if ( s8a > 5 ) // UndCC_FalseNeg, unless s8a volatile
    {
      // Will always get here.
    }
  }
}
