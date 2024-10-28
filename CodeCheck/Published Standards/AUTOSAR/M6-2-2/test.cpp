#include <cmath>
#include <limits>
void M6_2_2(){
  float x, y;
 
 

  // These next two are caught by the clang warning
  if ( x == y ); // UndCC_Violation
  if ( x == 0.1f ); // UndCC_Violation
  
  
  // This one is not caught by clang warning because 0 can be exactly represented in floating point
  if ( x == 0.0f ); // UndCC_Violation
  
  // An indirect test is equally problematic and is also prohibited by this rule:
  // Other indirect tests are not caught as there are infinite indirect tests
  if ( ( x >= y ) && ( x <= y ) ); // UndCC_Violation
  if ( ( 1.0 < y ) || ( y < 1.0 ) ); // UndCC_Violation
  
  // The following is better, but only if the magnitudes are appropriate:
  if ( std::abs( x - y ) <= std::numeric_limits<float>::epsilon( ) ); // UndCC_Valid
}
