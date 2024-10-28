// usingDirectiveDeclarationsNotInHeaderFiles2.cpp
#include "usingDirectiveDeclarationsNotInHeaderFiles2.h"
#include "usingDirectiveDeclarationsNotInHeaderFiles1.h"

void m2 ( )
{
  
  bar ( ); // bar calls foo ( int );
}
