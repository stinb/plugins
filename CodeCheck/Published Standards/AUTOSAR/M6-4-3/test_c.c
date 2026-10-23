/* Custom */

#define FLAG 1

extern void stop ( void );

void f1 ( int x, int y )
{
  switch ( x ) /* UndCC_Violation(c,cpp08) - default ends with return */
  {
    case 0:
      y = 1;
      break;
    default:
      y = 2;
      return;
  }

  switch ( x ) /* UndCC_Violation - default ends with a call, not break */
  {
    case 0:
      y = 1;
      break;
    default:
      stop();
  }

  switch ( x ) /* UndCC_Violation - return is not the last statement */
  {
    case 0:
      if ( y )
      {
        return;
      }
      y = 3;
    default:
      break;
  }

  switch ( x ) /* UndCC_Violation(c,cpp08) - case ends with goto */
  {
    case 0:
      y = 1;
      goto done;
    default:
      break;
  }

done:
  switch ( x ) /* UndCC_Violation - last clause is empty */
  {
    case 0:
      break;
    default:
  }

  switch ( x ) /* UndCC_Violation - do-while is the last statement */
  {
    case 0:
      do
      {
        break;
      } while ( y );
    default:
      break;
  }
}

void f2 ( int x, int y )
{
  switch ( x ) /* UndCC_Valid - break inside a compound clause */
  {
    case 0:
    {
      y = 1;
      break;
    }
    case 1:
    case 2:
      y = 2;
      break;
    case ( FLAG ? 3 : 4 ):
      break;
    default:
      break;
  }

  switch ( x ) /* UndCC_Valid - nested switch is complete */
  {
    case 0:
      switch ( y ) /* UndCC_Valid */
      {
        case 0:
          y = 1;
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

#define DONE break
#define PUT( size ) \
  switch ( size ) \
  { \
    case 1: \
      break; \
    default: \
      stop(); \
  }

void f4 ( int x, int y )
{
  switch ( x ) /* UndCC_FalsePos - break hidden in a macro is only seen when macro expansions are saved */
  {
    case 0:
      y = 1;
      DONE;
    default:
      DONE;
  }

  PUT( x ); /* UndCC_FalseNeg - a switch from a macro is only seen when macro expansions are saved */
}

void f5 ( int x, int y )
{
  switch ( x ) /* UndCC_Valid - directives after break are not statements */
  {
    case 0:
      y = 1;
      break;
#if FLAG
    case 1:
      y = 2;
      break;
#endif
    default:
      break;
  }
}
