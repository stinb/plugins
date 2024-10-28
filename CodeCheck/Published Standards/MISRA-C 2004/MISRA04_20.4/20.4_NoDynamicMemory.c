#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

typedef struct
{
   int ii;
   double dd;
} SSS;

int main()
{
   int kk, jj;

   char *str1 = "This is a text string";
   char *str2 = malloc(strlen(str1)); // UndCC_Violation

   SSS *s1 = calloc(4, sizeof(SSS)); // UndCC_Violation
   if (s1 == ENOMEM)
      printf("Error ENOMEM: Insufficient memory available\n");

   strcpy(str2, str1); /* Make a copy of the string */

   for (kk = 0; kk < 5; kk++)
   {
      s1[kk].ii = kk;
   }

   for (jj = 0; jj < 5; jj++)
   {
      printf("Value strored: %d\n", s1[jj].ii);
   }

   char *buffer;
   if ((buffer = (char *)realloc(str2, 15)) == NULL) // UndCC_Violation
   {
      printf("Reallocation failed\n");
      exit(1);
   }

   free(s1);   // UndCC_Violation
   free(str2); // UndCC_Violation
}
