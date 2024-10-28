// usingDirectiveDeclarationsNotInHeaderFiles1.cpp
#include "usingDirectiveDeclarationsNotInHeaderFiles1.h"
#include "usingDirectiveDeclarationsNotInHeaderFiles2.h"
int m1 ( )
{
  bar ( ); // bar calls foo ( char );
}

