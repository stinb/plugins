// MISRA C 2012

#include <stdint.h>

void f1 ( void )
{
 int32_t a1[ 10 ];
 int32_t a2[ 10 ];
 int32_t *p1 = a1;

 if ( p1 < a1 ) /* UndCC_Valid */
 {
 }
 if ( p1 < a2 ) /* UndCC_Violation */
 {
 }
}

struct limits
{
 int32_t lwb;
 int32_t upb;
};

void f2 ( void )
{
 struct limits limits_1 = { 2, 5 };
 struct limits limits_2 = { 10, 5 };

 if ( &limits_1.lwb <= &limits_1.upb ) /* UndCC_Valid */
 {
 }
 if ( &limits_1.lwb > &limits_2.upb )   /* UndCC_Violation */
 {
 }
}
