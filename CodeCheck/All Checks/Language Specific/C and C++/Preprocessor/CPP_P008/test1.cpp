// Custom

#define INVALID_PI 3.14159265 // UndCC_Violation

#define invalid_absolute_value( x ) ( ((x) < 0) ? -(x) : (x) ) // UndCC_Violation

#define invalid_count_up( v, low, high ) /* UndCC_Violation */ \
  for( (v) = (low); (v) <= (high); (v)++ )

#define OK_NO_TYPE // UndCC_Valid - allowed exception
