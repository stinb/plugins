#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>

typedef bool bool_t;

bool_t f ( uint8_t a )
{
  return ( isdigit ( ( int32_t ) a ) /* Compliant */
           && isalpha ( ( int32_t ) 'b' ) /* Compliant */
           /*&& islower ( EOF ) /* Compliant */
           && isalpha ( 256 ) ); /* UndCC_Violation */
}

void f(char c)
{
    if (isalpha((unsigned char)c)) { }   // UndCC_Valid
    if (isalpha((uint8_t)c)) { }   // UndCC_Valid
    if (isalpha(c)) { }   // UndCC_Violation
    if (isalpha(c + 1)) { }   // UndCC_Violation
}
void f1(int x)
{
    if (isdigit(x)) { }   // UndCC_Violation
}
void f2(void)
{
    (void) isalpha(256); // UndCC_Violation 
    (void) isdigit(-1); // UndCC_Violation 
}
