// Custom

#define NULL ((void *) 0)

void f ( )
{
  int x = 1;
  int y = 0;
  char *p, *s;

  if ( x < 0 )
  {
    // log_error( 3 );
    x = 0;
  }
  // else not needed

  if ( x < 0 )
  {
    // log_error ( 3 );
    x = 0;
  }
  else if ( y < 0 )
  {
    x = 3;
  }
  else  // this else clause is required, even if the
  {     // developer expects this will never be reached
    // No change in value of x
  }

  if ( x < 0 )
  {
    x = 0;
  }
  else if ( y < 0 )
  {
    x = 3;
  } // UndCC_Violation

  if ( x < 0 ) // UndCC_Valid
  {
    x = 0;
  }
  else
  {
    x = 3;
  }

  if ( x < 0 )
     x = 0;
  // else not needed

  if ( x < 0 )
    x = 0;
  else if ( y < 0 )
    x = 3; // UndCC_Violation

  if ( x < 0 )
  {
    x = 0;
    y = 10;
  }
  else if ( y < 0 )
    x = 3; // UndCC_Violation

  if ( x < 0 ) // UndCC_Valid
  {
    x = 0;
    y = 10;
  }
  else if ( y < 0 )
    x = 3;
  else
    x = 3;

  if (*p == '\\') {
    *s++ = *p++;
    if (*p)
      *s++ = *p++;
    else if (*s)
      *p++ = *s++; // UndCC_Violation
  } else if (strchr("+?|()", *p) != NULL)
  {
     x = 0;
  }  // UndCC_Violation

}
