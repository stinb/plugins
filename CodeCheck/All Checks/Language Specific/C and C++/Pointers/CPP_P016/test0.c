// MISRA C 2023

#include <stdint.h>

int32_t glbA[] = { 1, 2, 3, 4, 5 };

void f ( int32_t A[ 4 ] )
{
  /*
   * The following is non-compliant as it always gives the same answer,
   * irrespective of the number of members that appear to be in the array
   * (4 in this case), because A has type int32_t * and not int32_t[ 4 ].
   * As sizeof ( int32_t * ) is often the same as sizeof ( int32_t ),
   * numElements is likely to always have the value 1.
   */
  uint32_t numElements = sizeof ( A ) / sizeof ( int32_t ); // UndCC_Violation

  /*
   * The following is compliant as numElements_glbA will be given the
   * expected value of 5.
   */
  uint32_t numElements_glbA = sizeof ( glbA ) / sizeof ( glbA[ 0 ] );
}
