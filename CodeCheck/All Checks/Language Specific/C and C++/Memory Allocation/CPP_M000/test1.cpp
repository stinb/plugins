// Custom

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

typedef struct
{
   int ii;
   double dd;
} SSS;

int main()
{
   int kk, jj;

   char *str1 = "This is a text string";
   char *str2 = static_cast<char *>(malloc(strlen(str1))); // UndCC_Violation

   SSS *s1 = static_cast<SSS *>(calloc(4, sizeof(SSS))); // UndCC_Violation
   if (s1 == reinterpret_cast<SSS *>(ENOMEM))
      printf("Error ENOMEM: Insufficient memory available\n");

   strcpy(str2, str1);

   for (kk=0; kk < 5; kk++) {
      s1[kk].ii=kk;
   }

   for (jj = 0; jj < 5; jj++) {
      printf("Value strored: %d\n",s1[jj].ii);
   }

   free(s1);   // UndCC_Violation
   free(str2); // UndCC_Violation
}

void f1 ( )
{
   int32_t * i = new int32_t; // UndCC_Violation
   delete i;                  // UndCC_Violation
}
