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
	switch ( z ) /* UndCC_Valid */
	{
	 case 1:
	 z = 2;
	 break;
	 default:
	 z = 0;
	 break;
	}
}
