#define SUM(a,b,c) a + b + c // UndCC_Violation

void use_macro ( void )
{
#define SIZE 4			/* UndCC_Valid */
#define DATA 3			/* UndCC_Violation - DATA not used */
	use_int16 ( SIZE );
}
