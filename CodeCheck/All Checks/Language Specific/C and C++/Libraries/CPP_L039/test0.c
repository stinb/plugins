// MISRA C 2023

#include <stdio.h>

void error_action ( void );

void fn ( void )
{
  FILE *fp;
  void *p;

  fp = fopen ( "tmp", "w" );

  if ( fp == NULL )
  {
    error_action ( );
  }

  fclose ( fp );

  fprintf ( fp, "?" );    /* UndCC_Violation */
  p = fp;                 /* UndCC_Violation */
}
