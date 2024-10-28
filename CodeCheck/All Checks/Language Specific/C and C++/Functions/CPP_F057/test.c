#include <stdio.h>

int subNumbers(int a, int b)  //UndCC_Violation - if not in a code or header file
{
    int result;
    result = a-b;
    return result;   
}


//Bad function prologue
int multNumbers(int a, int b); //UndCC_Valid


int multNumbers(int a, int b)  //UndCC_Valid
{
    int result; // INLINE COMMENT NOT A PROLOGUE, SHOULD FLAG VIOLATION
    result = a*b;
    return result; 
}

