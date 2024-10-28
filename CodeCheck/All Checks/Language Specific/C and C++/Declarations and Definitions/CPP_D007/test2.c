// Custom

// #include "test1.h"

extern short count;
short count = 0;        /* UndCC_Valid */

unsigned short pressure = 101u; /* UndCC_Violation - no declaration prior to this definition */

unsigned short pressure2 = 102u;        // UndCC_Violation

extern void func1 ( void );
extern void func2 ( short x, short y );
extern void func3 ( short x, short y );

void func1 ( void )
{
  /* UndCC_Valid */
}

void func2 ( short x, short y )
{
  /* UndCC_Valid */
}

// Strict already checks for this
//void func3 ( short x, unsigned short y )
//{

//}

void func4 ( void ) // UndCC_Violation
{

}

static void func5 ( void )
{
  /* UndCC_Valid - rule does not apply to objects/functions with internal linkage */
}

void func6 ( void ) // UndCC_Violation
{
  /* UndCC_Valid -  Declaration in header file */
}

// Strict already checks for this
//int func7 ( void )
//{

//}
