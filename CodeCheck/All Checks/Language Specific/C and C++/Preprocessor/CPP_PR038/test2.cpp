// MISRA C++ 2023

#define M1( x, y ) ( x * y )         // UndCC_Violation - x not parenthesized

#define M2( x, y ) ( ( x ) * ( y ) ) // UndCC_Valid - x is directly parenthesized

#define M3( z ) z + 2                // UndCC_Violation - operator + is top-level

#define MY_ASSERT( cond )                 \
  do                                      \
  {                                       \
    if ( !cond ) /* UndCC_Violation */    \
    {                                     \
      std::cerr << #cond << " failed!\n"; \
      std::abort();                       \
    }                                     \
  } while( false )

#define PROP( Type, Name ) \
  Type Name;               \
  Type get_##Name() { return Name; }
