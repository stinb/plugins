// MISRA C 2012/2023

#include "Opaque.h"

void f ( void )
{
  pOpaqueType pObject;     /* UndCC_Valid - OpaqueType not defined */
  pObject = GetObject ( ); /* Get a handle to an OpaqueType object */
  UseObject ( pObject );   /* Use it... */
}
