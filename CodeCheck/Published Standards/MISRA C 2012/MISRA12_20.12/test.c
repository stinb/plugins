/* Examples taken from MISRA C 2012 / 2025 Rule 20.12 */

#include <stdint.h>

/* Object-like macros used as arguments to function-like macros below. */
#define AA      0xffff
#define BB_VAL  7

/* Variables that the BB() macro references after token-pasting. */
int32_t wowAA;
int32_t wowBB_VAL;
int32_t wow42;
int32_t wown;

/* Mixed-context macro: x is operand of ## (wow ## x) AND used elsewhere as
 * ( x ). The rule applies whenever a call-site argument is itself a macro. */
#define BB( x ) ( x ) + wow ## x

void f1 ( int32_t n )
{
    int32_t r;
    r = BB ( AA );          // UndCC_Violation - AA is a macro
    r = BB ( BB_VAL );      // UndCC_Violation - BB_VAL is a macro
    r = BB ( 42 );          // UndCC_Valid - literal, not subject to expansion
    r = BB ( n );           // UndCC_Valid - n is a variable
    (void)r;
}

/* MISRA-canonical compliant example: parameter X is mixed-context, but the
 * argument 'speed' is a variable (not subject to further macro replacement). */
int32_t speed;
int32_t speed_scale;
#define SCALE( X ) ( ( X ) * X ## _scale )

void f2 ( void )
{
    int32_t scaled_speed;
    scaled_speed = SCALE ( speed );   // UndCC_Valid - speed is a variable
    (void)scaled_speed;
}

/* Pure-operand parameter: only used as operand of # or ##. Rule never
 * applies; calls with macro arguments are still compliant. */
#define STR( X )       #X
#define CAT( A, B )    A ## B

int32_t wow1;

void f3 ( void )
{
    const char *s = STR ( AA );           // UndCC_Valid - X is operand-only
    int32_t v      = CAT ( wow, 1 );      // UndCC_Valid - A, B are operand-only
    (void)s; (void)v;
}

/* Pure-non-operand parameter: no # or ## present, so no mixed context. */
#define DBL( X ) ( ( X ) + ( X ) )

void f4 ( void )
{
    int32_t y = DBL ( AA );               // UndCC_Valid - no # or ##
    (void)y;
}

/* Multiple parameters where only one is mixed. Only the mixed one matters
 * for this rule; passing a macro for the operand-only parameter is fine. */
int32_t pre_m;
int32_t pre_AA;
#define MIX2( P, Q ) pre_ ## P + ( P ) + Q

void f5 ( int32_t m )
{
    int32_t r;
    r = MIX2 ( m, AA );        // UndCC_Valid - P is 'm' (variable), AA matches non-mixed Q
    r = MIX2 ( AA, m );        // UndCC_Violation - P is mixed and AA is a macro
    (void)r;
}
