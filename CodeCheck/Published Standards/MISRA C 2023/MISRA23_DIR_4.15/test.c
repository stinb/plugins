#include <math.h>
#include <fenv.h>

extern double get_result( void );
extern void use_result( double c );
extern double divisor( void );

void untested_call( double x )
{
  double a = sqrt( x );                 /* UndCC_Violation - a is never tested */
  use_result( a );
}

void nan_tested_only( double x )
{
  double a = log( x );                  /* UndCC_Violation - a may be infinite */
  if ( !isnan( a ) )
  {
    use_result( a );
  }
}

void infinity_tested_only( double x )
{
  double a = exp( x );                  /* UndCC_Violation - a may be a NaN */
  if ( !isinf( a ) )
  {
    use_result( a );
  }
}

void both_tested( double x )
{
  double a = pow( x, x );               /* UndCC_Valid */
  if ( !isnan( a ) && !isinf( a ) )
  {
    use_result( a );
  }
}

void finite_tested( double x )
{
  double a = sqrt( x );                 /* UndCC_Valid */
  if ( isfinite( a ) )
  {
    use_result( a );
  }
}

void bounded( double x )
{
  double a = log( x );                  /* UndCC_Valid */
  if ( a > 0.0 )
  {
    use_result( a );
  }
}

void constant_argument( void )
{
  double a = sqrt( 2.0 );               /* UndCC_Valid - Dir 4.11 judges a constant */
  use_result( a );
}

void result_consumed_directly( double x )
{
  use_result( sqrt( x ) );              /* UndCC_Violation - nothing tests the result */
}

void result_tested_directly( double x )
{
  if ( isfinite( sqrt( x ) ) )          /* UndCC_Valid */
  {
    use_result( 0.0 );
  }
}

void untested_division( double n )
{
  double d = get_result( );
  double q = n / d;                     /* UndCC_Violation - d may be zero */
  use_result( q );
}

void guarded_division( double n )
{
  double d = get_result( );
  if ( d != 0.0 )
  {
    use_result( n / d );                /* UndCC_Valid */
  }
}

void quotient_tested( double n )
{
  double d = get_result( );
  double q = n / d;                     /* UndCC_Valid */
  if ( isfinite( q ) )
  {
    use_result( q );
  }
}

void constant_divisor( double n )
{
  use_result( n / 2.0 );                /* UndCC_Valid */
}

void integer_division( int n, int d )
{
  use_result( ( double ) ( n / d ) );   /* UndCC_Valid - not a floating division */
}

void self_comparison( double x )
{
  double a = sqrt( x );                 /* UndCC_Violation - a may be infinite */
  if ( a != a )
  {
    return;
  }
  use_result( a );
}

void exception_flags_consulted( double x )
{
  double a = sqrt( x );                 /* UndCC_Valid - tested through the flags */
  if ( fetestexcept( FE_INVALID ) == 0 )
  {
    use_result( a );
  }
}

static long long g_elapsed;

void condition_guarded_global( int done )
{
  if ( g_elapsed && done )
  {
    use_result( ( double ) done / g_elapsed );   /* UndCC_Valid */
  }
}

void condition_guarded_local( double n )
{
  double d = get_result( );
  if ( d )
  {
    use_result( n / d );                  /* UndCC_Valid */
  }
}

void constant_divisor_variable( double n, int big )
{
  double scale = 1024.0;
  if ( big )
  {
    scale = 1048576.0;
  }
  use_result( n / scale );              /* UndCC_Valid - scale is never zero */
}

void reassigned_divisor( double n )
{
  double scale = 1024.0;
  scale = get_result( );
  use_result( n / scale );              /* UndCC_Violation - scale may be zero */
}

void constant_divisor_branches( double n, int big )
{
  double scale;
  if ( big )
  {
    scale = 1048576.0;
  }
  else
  {
    scale = 1024.0;
  }
  use_result( n / scale );              /* UndCC_Valid - scale is never zero */
}

void nan_test_does_not_guard_divisor( double n )
{
  double d = get_result( );
  if ( !isnan( d ) )
  {
    use_result( n / d );                /* UndCC_Violation - d may be zero */
  }
}

void bound_away_from_zero( double n )
{
  double d = get_result( );
  if ( d > 0.0 )
  {
    use_result( n / d );                /* UndCC_Valid */
  }
}

void bound_includes_zero( double n )
{
  double d = get_result( );
  if ( d >= 0.0 )
  {
    use_result( n / d );                /* UndCC_Violation - d may be zero */
  }
}

void magnitude_guarded( double n )
{
  double d = get_result( );
  if ( fabs( d ) > 1e-9 )
  {
    use_result( n / d );                /* UndCC_Valid */
  }
}

double handed_to_the_caller( double x )
{
  return sqrt( x );                     /* UndCC_Violation - not tested here */
}

void tested_after_the_use( double x )
{
  double a = log( x );                  /* UndCC_FalseNeg - the test follows the use */
  use_result( a );
  if ( !isfinite( a ) )
  {
    return;
  }
}

void divisor_is_not_a_variable( double n, double d )
{
  use_result( n / ( d + 1.0 ) );        /* UndCC_FalseNeg - the divisor is not a variable */
}

void overflow_by_multiplication( double x )
{
  double a = x * x;                     /* UndCC_FalseNeg - only division and calls are modelled */
  use_result( a );
}

void early_return_guard( double n, int count )
{
  if ( count < 8 )
  {
    return;
  }
  use_result( n / count );              /* UndCC_Valid - count is 8 or more */
}

void reversed_bound( double n )
{
  double d = get_result( );
  if ( 5.0 > d )
  {
    use_result( n / d );                /* UndCC_Violation - d may be zero */
  }
}

void reversed_bound_away_from_zero( double n )
{
  double d = get_result( );
  if ( 0.0 < d )
  {
    use_result( n / d );                /* UndCC_Valid */
  }
}

void use_on_the_rejected_branch( double n )
{
  double d = get_result( );
  if ( d != 0.0 )
  {
    return;
  }
  use_result( n / d );                  /* UndCC_FalseNeg - d is zero here */
}

void use_after_a_rejecting_bound( double n )
{
  double d = get_result( );
  if ( d > 0.0 )
  {
    return;
  }
  use_result( n / d );                  /* UndCC_FalseNeg - d may be zero */
}
