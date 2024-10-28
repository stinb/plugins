#include <stdio.h>

#define IF if /* Bad macros */ // UndCC_Violation
#define THEN { // UndCC_Violation
#define ELSE } else { // UndCC_Violation
#define ENDIF } // UndCC_Violation
#define BEGIN { // UndCC_Violation
#define END } // UndCC_Violation

FILE * open_file (
  char *file_name
)

BEGIN

  FILE *file_ptr = NULL; /* Pointer to the results file */

  IF ((file_ptr = fopen(file_name, "w")) == NULL)
  THEN
    printf("Could not open file\n");
  ELSE
    printf("File %s was opened successfully\n", file_name);
  ENDIF

  return file_ptr;

END /* end of open_file */
