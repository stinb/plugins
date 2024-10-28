#include <stdint.h>
struct s; /* Incomplete type */
struct t; /* A different incomplete type */
struct s *sp;
struct t *tp;
int16_t *ip;
#include <stdlib.h> /* To obtain macro NULL */
int main() {
  ip = ( int16_t * ) sp; /* UndCC_Violation */
  sp = ( struct s * ) 1234; /* UndCC_Violation */
  tp = ( struct t * ) sp; /* UndCC_Violation - casting pointer into a
  * different incomplete type */
  sp = NULL; /* UndCC_Valid - exception 1 */
}
struct s *f ( void );
//(void) f ( ); /* UndCC_Valid - exception 2 commented out for error*/

