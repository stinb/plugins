void f1661 ( )
{
  int j661 = 0;
  goto L1661;
  for ( j661 = 0; j661 < 10 ; ++j661 )
  {
L1661: // UndCC_Violation
    j661;
  }
}

void f2661 ( )
{
  for ( int j661 = 0; j661 < 10 ; ++j661 )
  {
    for ( int i661 = 0; i661 < 10; ++i661 )
    {
      goto L1661;
    }
  }
L1661: // UndCC_Valid
  f1661 ( );
}

void f3661 ( )
{
	if ( true )
	{
L1661: // UndCC_Violation
		int test661 = 0;
	}
	int j661 = 0;
	goto L1661;
	for ( j661 = 0; j661 < 10 ; ++j661 )
	{
		j661--;
	}
}
