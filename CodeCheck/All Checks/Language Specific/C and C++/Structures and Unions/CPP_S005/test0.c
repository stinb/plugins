// MISRA C 2023

#include <stdint.h>
#include <string.h>

typedef _Bool bool_t;

struct S
{
  uint32_t x;
};

struct S;

/*
* Return value may indicate that 's1' and 's2' are different due to padding.
*/
bool_t f1 ( struct S *s1, struct S *s2 )
{
  return ( memcmp ( s1, s2, sizeof ( struct S ) ) != 0 ); /* UndCC_Violation */
}

union U
{
  uint32_t range;
  uint32_t height;
};

/*
* Return value may indicate that 'u1' and 'u2' are the same
* due to unintentional comparison of 'range' and 'height'.
*/
bool_t f2 ( union U *u1, union U *u2 )
{
  return ( memcmp ( u1, u2, sizeof ( union U ) ) != 0 ); /* UndCC_Violation */
}

const char a[ 6 ] = "task";

/*
* Return value may incorrectly indicate strings are different as the
* length of 'a' (4) is less than the number of bytes compared (6).
*/
bool_t f3 ( const char b[ 6 ] )
{
  return ( memcmp ( a, b, 6 ) != 0 ); /* UndCC_Violation */
}
