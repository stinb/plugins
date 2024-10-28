extern int b_32;
void f_32 ( void )
{
	unsigned short x_32 = 0; // comment UndCC_Violation \
	if ( b_32 )
	{
		++x_32; /* This is always executed */
	}
}
