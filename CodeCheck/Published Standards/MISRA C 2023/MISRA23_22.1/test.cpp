#include <stdlib.h>
#include <stdio.h>
int main ( void )
{
  void *b = malloc ( 40 ); // UndCC_Violation - dynamic memory not released 
  return 1;
}

int fun(void) {
    FILE *fp = fopen("tmp", "r"); // UndCC_Violation
    
    if (fp == NULL) {
        return 1;  // Error opening file
    }
    
    return 0;
}

int fun2(void) {
  FILE *fp;
  fp = fopen ( "tmp-1", "w" );
  fprintf ( fp, "*" );
  /* UndCC_FalseNeg File "tmp-1" should be closed here, but stream 'leaks' */
  fp = fopen ( "tmp-2", "w" );
  fprintf ( fp, "!" );
  fclose ( fp );
  return ( 0 );
}
