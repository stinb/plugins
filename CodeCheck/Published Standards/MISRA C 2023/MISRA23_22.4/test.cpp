#include <stdio.h>
void fn ( void )
{
  FILE *fp = fopen ( "tmp", "r" );
  ( void ) fprintf ( fp, "What happens now?" );  // UndCC_Violation
  ( void ) fclose ( fp );
  const char *mode = "r";
  fp = fopen("data.txt", mode);
  fprintf(fp, "Hello"); // UndCC_Violation

}

