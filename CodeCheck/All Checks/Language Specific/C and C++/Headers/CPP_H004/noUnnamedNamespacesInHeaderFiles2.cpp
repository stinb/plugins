// noUnnamedNamespacesInHeaderFiles2.cpp
#include "noUnnamedNamespacesInHeaderFiles.h"
extern void fn_a();
namespace
{
  int x;
}
void fn_b ( void )
{
  fn_a ( );
  if ( x == 24 ) // This x will not have been initialized.
  {
  }
}
