#include <stdlib.h>
void fn ( void )
{
  int32_t a;
  free ( &a ); // UndCC_Violation - a does not point to allocated storage
}
void g ( void )
{
  char *p = ( char * ) malloc ( 512 );
  char *q = p;
  free ( p );
  free ( q ); // UndCC_Violation - allocated block freed a second time 
  p = ( char * ) realloc ( p, 1024 ); // UndCC_FalseNeg - allocated block may be freed a third time
}

