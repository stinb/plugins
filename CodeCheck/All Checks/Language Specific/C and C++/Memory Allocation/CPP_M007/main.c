#include <stdio.h>
#include <stdlib.h>

void GetMatches ( const char * format, ... );

int main() {
  
  /* scanf() and fscanf() */
  FILE *fp;
  char name[50];
  int roll_no, chars;
  float marks;
  char str1[20], str2[20], str3[20], str4[20];

  printf("Enter name: ");
  scanf("%5s", str1);                // valid
  
  
  printf("Enter occupation: ");
  scanf("%s", str2);                 // invalid // UndCC_Violation
  
  printf("Enter country: ");
  scanf("%[...]", str3);             // invalid // UndCC_Violation
  
  
  printf("Enter favorite food: ");
  scanf("%10[^...]", str4);          // valid


  fp = fopen("records.txt", "r");

  if(fp == NULL)
  {
    printf("Error opening file\n");
    exit(1);
  }

  printf("Testing fscanf() function: \n\n");
  printf("Name:\t\tRoll\t\tMarks\n");

  while( fscanf(fp, "Name: %s\t\tRoll no: %d\t\tMarks: %f\n" // UndCC_Violation
                , name, &roll_no, &marks) != EOF )                     // invalid
  {
    printf("%s\t\t%d\t\t%.2f\n", name, roll_no ,marks);
  }

  fclose(fp);
  
  /* sscanf() */
  char* buffer = "Hello World";
  char store_hello[10], store_world[10];
  int total_read;

  total_read = sscanf(buffer, "%s %s" , store_hello, store_world);      // invalid // UndCC_Violation
  
  /* vscanf() and vfscanf() */
  int val;
  char str[100];

  printf ("Please enter a number and a word: ");
  fflush (stdout);
  GetMatches (" %d %99s ", &val, str);
  printf ("Number read: %d\nWord read: %s\n", val, str);
  
  return 0;
}

/* False negative with vscanf() passed a string indirectly */
void GetMatches ( const char * format, ... )
{
  va_list args;
  va_start (args, format);
  vscanf (format, args);
  va_end (args);
}
