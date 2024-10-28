void test( int x, y, z) 
{
	switch ( x ) // UndCC_Violation
	{
	 default: 
	 x = 0;
	 break;
	}
	switch ( y ) // UndCC_Violation
	{
	 case 1:{}
	 default: 
	 y = 0;
	 break;
	}
	switch ( z )
	{
	 case 1:
	 z = 2;
	 break;
	 default: /* UndCC_Valid */
	 z = 0;
	 break;
	}
}
