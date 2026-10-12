#include <cstdint>

/* X is mixed-use: an operand of ## and used on its own. */
#define SCALE( X ) ( ( X ) * X ## _scale )

/* Y is only ever an operand of #, so it is not mixed-use. */
#define SHOW( Y ) #Y

/* Z is never an operand, so it is not mixed-use. */
#define TWICE( Z ) ( ( Z ) + ( Z ) )

#define AA BB

std::int32_t speed;
std::int32_t speed_scale;
std::int32_t AA_scale = 1;
std::int32_t BB = 42;
std::int32_t BB_scale = 2;

std::int32_t scaled_speed = SCALE( speed );   /* UndCC_Valid - speed is not a macro */
std::int32_t scaled_AA = SCALE( AA );         /* UndCC_Violation */

const char *shown = SHOW( AA );               /* UndCC_Valid - Y is not mixed-use */
std::int32_t doubled = TWICE( AA );           /* UndCC_Valid - Z is not mixed-use */

/* Both parameters are mixed-use. */
#define PAIR( A, B ) ( ( A ) + ( B ) + A ## _scale + B ## _scale )

std::int32_t pair_ok = PAIR( speed, BB );     /* UndCC_Valid */
std::int32_t pair_bad = PAIR( speed, AA );    /* UndCC_Violation - the second argument */

/* X is mixed-use through # rather than ##, so the argument may be an
   expression. A macro name anywhere inside it is still expanded. */
#define LABEL( X ) ( #X, ( X ) )

std::int32_t plain = ( LABEL( speed + 1 ), 0 );   /* UndCC_Valid */
std::int32_t nested = ( LABEL( AA + 1 ), 0 );     /* UndCC_Violation */

/* A # inside a string literal is not a stringize operator, so Q is used
   plainly only and the macro has no mixed-use parameter. */
#define QUOTED( Q ) ( "#Q", ( Q ) )

std::int32_t quoted = ( QUOTED( AA ), 0 );    /* UndCC_Valid */
