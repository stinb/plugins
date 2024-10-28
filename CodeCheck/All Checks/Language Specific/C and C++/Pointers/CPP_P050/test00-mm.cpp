#include <stdint.h>
#include <cstdlib>

int fint (int z){return z;}
void f0 (int a, long* b, char* c, int32_t * d){} /* UNDCC_Valid */
void f1 ( int32_t*); /* UNDCC_Valid */
void f2 (int* a, bool b, int * c= NULL, int = 0, int * d = 0) {} /* UNDCC_Violation 4 not compliant */
void f3 ( )
{
   f0 (fint(0), 0, NULL, NULL ); /* UNDCC_Violation 1 */
   f1 ( 0 ); /* UNDCC_Violation 0 */

   f2(0, 1); /* UNDCC_Violation 0 */
   f2(NULL, 0, 0, 0); /* UNDCC_Violation 2 */
}
