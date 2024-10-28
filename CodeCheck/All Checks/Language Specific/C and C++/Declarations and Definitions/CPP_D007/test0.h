/* MISRA C 2012 */

#include <stdint.h>

extern  int16_t count;
        int16_t count = 0;        /* UndCC_Valid                        */

extern uint16_t speed = 6000u;    /* UndCC_FalseNeg - no declaration
                                   * prior to this definition,
                                   * but variable is typed              */

uint8_t pressure = 101u;          /* UndCC_FalseNeg - no declaration
                                   * prior to this definition,
                                   * but variable is typed              */

extern void func1 ( void );
extern void func2 ( int16_t x, int16_t y );
extern void func3 ( int16_t x, int16_t y );

void func1 ( void )                  /* UndCC_Valid */
{

}

void func2 ( int16_t x, int16_t y )  /* UndCC_Valid */
{

}

// void func3 ( int16_t x, uint16_t y ) /* Analysis error - parameter types different */
// {

// }

void func4 ( void )                  /* UndCC_Violation - no declaration of func4 before this definition */
{

}

static void func5 ( void )           /* UndCC_Valid - rule does not apply to objects/functions with internal linkage */
{

}
