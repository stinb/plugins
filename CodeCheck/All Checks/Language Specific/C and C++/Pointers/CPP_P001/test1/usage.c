// Custom

#include "types.h"
char * const a; /* Standard pointers should be ignored */
const char * b;

void f ( void )
{
  pOpaqueType pObject;
  pObject = GetObject( );   /* Get a handle to an OpaqueType object */
  UseObject( pObject );     /* Use it... */

  pVisibleType pObject2;    /* UndCC_Violation: pObject2 can see implementation, but is never dereferenced */
  pObject2 = GetObject2( ); /* Get a handle to an VisibleType object */
  UseObject2( pObject2 );   /* Use it... */

  pVisibleTypeDeref pObject3;
  pObject3 = GetObject3( ); /* Get a handle to an OpaqueType object */
  pObject3->x = 3;          /* Deref Use it... */
}
