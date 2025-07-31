#include <cstdint>
char *s = "string"; // UndCC_Violation
/* Compliant - p is const-qualified; additional qualifiers are permitted */
const volatile char *p = "string";
extern void f1 ( char *s1 );
extern void f2 ( const char *s2 );
void g ( void )
{
  f1 ( "string" ); // UndCC_Violation 
  f2 ( "string" ); /* UndCC_Valid */
}
char *name1 ( void )
{
  return ( "MISRA" ); // UndCC_Violation
}
const char *name2 ( void )
{
  return ( "MISRA" ); /* UndCC_Valid */
}
extern void f3( uint16_t x, ... ); /* Note: non-compliant with Rule 17.1 */
extern void f4( char *text, ... ); /* Note: non-compliant with Rule 17.1 */
void variadic( void )
{
  f3( 42u, "MISRA" ); /* UndCC_Valid by exception */
  f4( "MISRA", 42u ); /* UndCC_Violation - exception only applies to
variable argument lists */
}

