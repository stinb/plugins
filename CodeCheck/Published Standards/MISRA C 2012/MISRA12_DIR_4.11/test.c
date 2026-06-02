/* Test for MISRA C 2012/2023/2025 Directive 4.11 - The validity of values
 * passed to library functions shall be checked.
 *
 * Conservative subset: a Standard Library call is flagged only when an
 * argument is a compile-time constant that is provably outside the function's
 * required input domain. Non-constant arguments are not analysed (documented
 * false negatives) and therefore carry no violation marker.
 *
 * Prototypes are declared manually rather than via system headers so the test
 * is portable and so the ctype functions are real calls rather than macros.
 */

/* <math.h> */
extern double sqrt  ( double );
extern float  sqrtf ( float );
extern long double sqrtl ( long double );
extern double log   ( double );
extern double log10 ( double );
extern double log2  ( double );
extern double asin  ( double );
extern double acos  ( double );
extern double acosh ( double );
extern double atanh ( double );
extern double log1p ( double );
extern double atan2 ( double, double );
extern double pow   ( double, double );
extern double fmod  ( double, double );
extern double remainder ( double, double );
extern double remquo ( double, double, int * );
extern double sin   ( double );

/* <ctype.h> */
extern int isalpha ( int );
extern int isdigit ( int );
extern int isspace ( int );
extern int toupper ( int );
extern int tolower ( int );

/* <stdlib.h> / <inttypes.h> */
extern int       abs     ( int );
extern long      labs    ( long );
extern long long llabs   ( long long );
extern long long imaxabs ( long long );  /* intmax_t modeled as long long */

extern void use_d ( double );
extern void use_i ( int );

/* ===== sqrt family: negative argument ===== */
void test_sqrt ( double x )
{
    use_d ( sqrt  ( -1.0 ) );      /* UndCC_Violation - negative                   */
    use_d ( sqrt  ( -1 ) );        /* UndCC_Violation - negative (integer literal) */
    use_d ( sqrtf ( -2.0f ) );     /* UndCC_Violation - negative                   */
    use_d ( sqrtl ( -3.0L ) );     /* UndCC_Violation - negative                   */

    use_d ( sqrt  ( 4.0 ) );       /* UndCC_Valid - non-negative                   */
    use_d ( sqrt  ( 0.0 ) );       /* UndCC_Valid - zero is in domain              */
    use_d ( sqrt  ( x ) );         /* UndCC_Valid - not a constant, not analysed   */
    if ( x >= 0.0 ) {
        use_d ( sqrt ( x ) );      /* UndCC_Valid - value checked before use       */
    }
}

/* ===== log family: non-positive argument ===== */
void test_log ( void )
{
    use_d ( log   ( -1.0 ) );      /* UndCC_Violation - negative                   */
    use_d ( log   ( 0.0 ) );       /* UndCC_Violation - zero (pole error)          */
    use_d ( log10 ( -5.0 ) );      /* UndCC_Violation - negative                   */
    use_d ( log2  ( 0.0 ) );       /* UndCC_Violation - zero                        */

    use_d ( log   ( 1.0 ) );       /* UndCC_Valid - positive                       */
    use_d ( log10 ( 100.0 ) );     /* UndCC_Valid - positive                       */
}

/* ===== asin / acos: argument outside [-1, 1] ===== */
void test_asin_acos ( void )
{
    use_d ( acos ( 2.0 ) );        /* UndCC_Violation - greater than 1             */
    use_d ( acos ( -2.0 ) );       /* UndCC_Violation - less than -1               */
    use_d ( asin ( 1.5 ) );        /* UndCC_Violation - greater than 1             */
    use_d ( asin ( -1.01 ) );      /* UndCC_Violation - less than -1               */

    use_d ( acos ( 1.0 ) );        /* UndCC_Valid - boundary                       */
    use_d ( acos ( -1.0 ) );       /* UndCC_Valid - boundary                       */
    use_d ( asin ( 0.5 ) );        /* UndCC_Valid - in domain                      */
}

/* ===== fmod / remainder / remquo: zero second argument ===== */
void test_fmod ( double x )
{
    int q;

    use_d ( fmod ( 10.0, 0.0 ) );  /* UndCC_Violation - second argument is zero    */
    use_d ( fmod ( x, 0.0 ) );     /* UndCC_Violation - second argument is zero    */
    use_d ( remainder ( 10.0, 0.0 ) );    /* UndCC_Violation - zero divisor        */
    use_d ( remquo ( 10.0, 0.0, &q ) );   /* UndCC_Violation - zero divisor        */

    use_d ( fmod ( 10.0, 3.0 ) );  /* UndCC_Valid - non-zero divisor               */
    use_d ( fmod ( 0.0, 5.0 ) );   /* UndCC_Valid - first argument may be zero     */
    use_d ( fmod ( 10.0, x ) );    /* UndCC_Valid - divisor not a constant         */
    use_d ( remainder ( 10.0, 3.0 ) );    /* UndCC_Valid - non-zero divisor        */
}

/* ===== acosh / atanh / log1p / atan2: other restricted math domains ===== */
void test_more_math ( double x )
{
    use_d ( acosh ( 0.5 ) );       /* UndCC_Violation - less than 1                */
    use_d ( atanh ( 1.0 ) );       /* UndCC_Violation - not strictly inside -1..1  */
    use_d ( atanh ( -2.0 ) );      /* UndCC_Violation - outside -1..1              */
    use_d ( log1p ( -1.0 ) );      /* UndCC_Violation - pole at -1                 */
    use_d ( log1p ( -2.0 ) );      /* UndCC_Violation - below -1                   */
    use_d ( atan2 ( 0.0, 0.0 ) );  /* UndCC_Violation - both arguments zero        */

    use_d ( acosh ( 1.0 ) );       /* UndCC_Valid - boundary                       */
    use_d ( atanh ( 0.5 ) );       /* UndCC_Valid - inside domain                  */
    use_d ( log1p ( 0.0 ) );       /* UndCC_Valid - above -1                       */
    use_d ( atan2 ( 0.0, 1.0 ) );  /* UndCC_Valid - not both zero                  */
    use_d ( atan2 ( x, 0.0 ) );    /* UndCC_Valid - first argument not a constant  */
}

/* ===== pow: negative base with non-integer exponent, or zero base / neg exp == */
void test_pow ( double x )
{
    use_d ( pow ( -2.0, 0.5 ) );   /* UndCC_Violation - negative base, non-integer exponent */
    use_d ( pow ( -8.0, 0.25 ) );  /* UndCC_Violation - negative base, non-integer exponent */
    use_d ( pow ( 0.0, -1.0 ) );   /* UndCC_Violation - zero base, negative exponent (pole) */

    use_d ( pow ( -2.0, 3.0 ) );   /* UndCC_Valid - negative base, integer exponent */
    use_d ( pow ( 2.0, 0.5 ) );    /* UndCC_Valid - positive base                  */
    use_d ( pow ( 0.0, 2.0 ) );    /* UndCC_Valid - zero base, positive exponent   */
    use_d ( pow ( 0.0, 0.0 ) );    /* UndCC_Valid - defined as 1                   */
    use_d ( pow ( x, 0.5 ) );      /* UndCC_Valid - base not a constant            */
}

/* ===== <ctype.h>: argument not representable as unsigned char or EOF ===== */
void test_ctype ( int c )
{
    use_i ( toupper ( -2 ) );      /* UndCC_Violation - below EOF                  */
    use_i ( tolower ( 256 ) );     /* UndCC_Violation - above UCHAR_MAX            */
    use_i ( isalpha ( 300 ) );     /* UndCC_Violation - above UCHAR_MAX            */
    use_i ( isdigit ( -5 ) );      /* UndCC_Violation - below EOF                  */

    use_i ( toupper ( -1 ) );      /* UndCC_Valid - EOF is permitted               */
    use_i ( toupper ( 'a' ) );     /* UndCC_Valid - in unsigned char range         */
    use_i ( isspace ( 0 ) );       /* UndCC_Valid - in range                       */
    use_i ( isalpha ( 255 ) );     /* UndCC_Valid - UCHAR_MAX                       */
    use_i ( isdigit ( c ) );       /* UndCC_Valid - not a constant, not analysed   */
}

/* ===== abs: most negative integer ===== */
void test_abs ( int n )
{
    use_i ( abs ( -2147483647 - 1 ) ); /* UndCC_Violation - INT_MIN, undefined     */

    /* llabs / imaxabs take a 64-bit parameter: the most negative value is derived
     * from the parameter type, not from int, so these flag at LLONG_MIN rather
     * than INT_MIN.                                                              */
    use_i ( (int) llabs   ( -9223372036854775807LL - 1LL ) ); /* UndCC_Violation - LLONG_MIN, undefined */
    use_i ( (int) imaxabs ( -9223372036854775807LL - 1LL ) ); /* UndCC_Violation - most negative, undefined */

    use_i ( abs  ( 5 ) );          /* UndCC_Valid - in range                       */
    use_i ( abs  ( -5 ) );         /* UndCC_Valid - has a positive counterpart     */
    use_i ( abs  ( n ) );          /* UndCC_Valid - not a constant, not analysed   */
    use_i ( (int) labs   ( 5L ) ); /* UndCC_Valid - in range                       */
    use_i ( (int) llabs  ( -2147483648LL ) ); /* UndCC_Valid - INT_MIN fits in long long */
}

/* ===== Not analysed: trig precision and user-defined lookalikes ===== */
double sqrt_local ( double v );    /* user function, file scope but not the library */

void test_not_flagged ( void )
{
    use_d ( sin ( 1000.0 ) );      /* UndCC_Valid - large-arg precision not analysed */
    use_d ( sqrt_local ( -1.0 ) ); /* UndCC_Valid - user-defined function, not libc  */
}
