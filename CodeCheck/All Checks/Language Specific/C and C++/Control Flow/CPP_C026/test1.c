// MISRA 2012 15.4

#include <stdbool.h>

bool ExitNow ( int x ) { return true; }
bool BreakNow ( int x ) { return true; }
bool GotoNow ( int x ) { return true; }
void KeepGoing ( int x ) { }
int calc_new_x ( ) { return 0; }
int calc_new_y ( ) { return 0; }

void test1 ()
{
  int x, y, z;
  int LIMIT = 10;

  // Both of the following nested loops are compliant as each has a single break
  // used for early loop termination.
  for ( x = 0; x < LIMIT; ++x ) // UndCC_Valid - 1 break
  {
    if ( ExitNow ( x ) )
    {
      break;
    }
    for ( y = 0; y < x; ++y )   // UndCC_Valid - 1 break
    {
      if ( ExitNow ( LIMIT - y ) )
      {
        break;
      }
    }
  }

  // The following loop is non-compliant as there are multiple break and goto
  // statements used for early loop termination.
  for ( x = 0; x < LIMIT; ++x ) // UndCC_Violation(both) - 1 break and 1 goto
  {
    if ( BreakNow ( x ) )
    {
      break;
    }
    else if ( GotoNow ( x ) )
    {
      goto EXIT;
    }
    else
    {
      KeepGoing ( x );
    }
  }
  EXIT:
    ;

  // In the following example, the inner while loop is compliant because there
  // is a single goto statement that can cause its early termination. However,
  // the outer while loop is non-compliant because it can be terminated early
  // either by the break statement or by the goto statement in the inner while
  // loop.
  while ( x != 0u )             // UndCC_Violation(both) - 1 break and 1 goto
  {
    x = calc_new_x ( );
    if ( x == 1u )
    {
      break;
    }
    while ( y != 0u )
    {
      y = calc_new_y ( );
      if ( y == 1u )
      {
        goto L1;
      }
    }
  }
  L1:
  z = x + y;
}
