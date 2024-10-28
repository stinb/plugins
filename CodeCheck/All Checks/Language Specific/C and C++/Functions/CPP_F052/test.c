#include <stdio.h>

#include <stdio.h> 
#define MAX 15 
  
int main() 
{ 
    char buf[MAX]; 
  
    printf("Enter a string: "); 
    gets(buf);                         // invalid use of gets // UndCC_Violation
    printf("string is: %s\n", buf); 

    fgets(buf, MAX, stdin);            // valid use of fgets
    printf("string is: %s\n", buf);
  
    return 0; 
} 