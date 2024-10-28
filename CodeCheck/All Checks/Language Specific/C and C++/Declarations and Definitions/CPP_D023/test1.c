// Custom

int N = 10;	// UndCC_Violation - only used in func
int M;		  // UndCC_Violation - only used in func
int L;		  // UndCC_Valid - used in multiple functions

void func ( void )
{
	M = 11;
	L = 12;
	int i;
	for ( i = 0; i < N; ++i )
	{
	}
}

int count ( void )
{
	L = 12;
	static unsigned int call_count = 0;
	++call_count;
	return call_count;
}
