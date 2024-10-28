// MISRA C 2012/2023 - Rule 8.2

#include <stdint.h>

// Compliant
extern int16_t func1 ( int16_t n );

// Non-compliant - parameter name not specified
extern void func2 ( int16_t ); // UndCC_Violation

// Non-compliant - not in prototype form
static int16_t func3 ( ); // UndCC_Violation

// Compliant - prototype specifies 0 parameters
static int16_t func4 ( void );

// Compliant
int16_t func1 ( int16_t n )
{
  return n;
}

// Non-compliant - old style identifier and declaration list
static int16_t func3 ( vec, n ) // UndCC_Violation
int16_t *vec;
int16_t n;
{
  return vec[ n - 1 ];
}

// Non-compliant - no prototype
int16_t ( *pf1 ) ( ); // UndCC_Violation

// Compliant - prototype specifies 0 parameters
int16_t ( *pf1 ) ( void );

// Non-compliant - parameter name not specified
typedef int16_t ( *pf2_t ) ( int16_t ); // UndCC_Violation

// Compliant
typedef int16_t ( *pf3_t ) ( int16_t n );
