// noUnnamedNamespacesInHeaderFiles1.cpp
#include "noUnnamedNamespacesInHeaderFiles.h"
namespace
{
  int x;
}
void fn_a ( void )
{
  x = 24;
}
