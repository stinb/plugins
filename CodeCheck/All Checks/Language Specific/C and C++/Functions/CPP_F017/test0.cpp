#include <cstdint>

// MISRA C++ 2008

template <typename T> void f ( T );  // overload Example 1
template <typename T> void f ( T* ); // overload Example 2
template <> void f<int32_t*> ( int32_t* ); // UndCC_Violation - explicit specialization of
                                           // Example 1
void b ( int32_t * i )
{
  f ( i ); // (Violation above in declaration)
           // - Calls Example 2, f<int32_t*>
}


template <typename T> void g ( T );        // Example 1
template <> void g<int32_t*> ( int32_t* ); // UndCC_Violation(1) Example 2
void c ( int32_t * i )
{
 g ( i ); // UndCC_Valid
          // - Calls Example 2, g<int32_t*>
}
