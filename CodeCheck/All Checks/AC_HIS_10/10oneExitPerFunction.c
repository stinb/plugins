#define MAX 10
#define NULL 0

int multipleExits( unsigned n, char *p ) { // UndCC_Violation
	
	if ( n > MAX )
	{
		return 0;
	}
	
	if ( p == NULL )
	{
		return 0;
	}
	return 1;
}
