// Custom

void f1 ( int a )
{
  if ( a <= 0 )
  {
    goto L2;         // UndCC_Violation
  }

  goto L1;           // UndCC_Valid

  if ( a == 0 )
  {
    goto L1;         // UndCC_Valid
  }

  goto L2;           // UndCC_Violation

L1:
  if ( a > 0 )
  {
  L2:
    goto L1;         // UndCC_Valid
  }
  {
    {
      goto L2;       // UndCC_Violation
    }
  }

  int x = 1;
  int y = 1;
  goto L4;           // UndCC_Valid
L5:
  switch ( x )
  {
    case 0:
      if ( x == y )
      {
        goto L3;     // UndCC_Violation
      }
      break;
    case 1:
      y = x;
L3:
      ++x;
      break;
    case 2:
      goto L4;       // UndCC_Valid
      goto L5;       // UndCC_Valid
    case 3:
      goto L3;       // UndCC_Violation
    default:
      break;
  }

L4:
  return;
}

void f2 ( int x )
{
  switch ( x )
  {
    case -1:
fail:
    {
      break;
    }
    default:
      goto fail; // UndCC_Violation
  }
}
