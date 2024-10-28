// MISRA C 2004 and MISRA C++ 2008

void a( int x )
{
  if ( x < 0 )
  {
    log_error(3);
    x = 0;
  } /*  else not needed */
}

void b( int x, int y )
{
  if ( x < 0 )
  {
    log_error(3);
    x = 0;
  }
  else if ( y < 0 )
  {
    x = 3;
  }
  else /* this else clause is required, even if the */
  {    /* programmer expects this will never be reached */
       /* no change in value of x */
  }
}
