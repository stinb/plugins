#include <locale.h>
#include <stdio.h>
#include <time.h>

int main () {
   time_t currtime;
   struct tm *timer;
   char buffer[80];

   time( &currtime );
   timer = localtime( &currtime );  // UndCC_Violation

   printf("Locale is: %s\n", setlocale(LC_ALL, "en_GB"));  // UndCC_Violation
   strftime(buffer,80,"%c", timer );
   printf("Date is: %s\n", buffer);

  
   printf("Locale is: %s\n", setlocale(LC_ALL, "de_DE"));  // UndCC_Violation
   strftime(buffer,80,"%c", timer );
   printf("Date is: %s\n", buffer);

   return(0);
}
