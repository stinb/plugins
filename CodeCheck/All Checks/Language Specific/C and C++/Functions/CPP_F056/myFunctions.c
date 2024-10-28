#include <stdio.h>

// VALID PROLOGUE
/*
 * Definition for addNumbers().
 * 
 * This is a valid prologue
 *
 */
int addNumbers(int a, int b) //UndCC_Valid
{
    int result;
    result = a+b;
    return result;   
}


// VALID PROLOGUE
/*****************************************
* Function name: [function name]
* Description: [description]
* Notes: [notes]
* Arguments: [arguments]
* Return value: [return value]
* Errors: [errors]
* References: [references]
******************************************/
int subNumbers(int a, int b)  //UndCC_Valid
{
    int result;
    result = a-b;
    return result;   
}


// VALID PROLOGUE
/*****************************************
* Function name: [function name]
* Description: [description]
* Notes: [notes]
* Arguments: [arguments]
* Return value: [return value]
* Errors: [errors]
* References: [references]
******************************************/
int multNumbers(int a, int b);


int multNumbers(int a, int b)  //UndCC_Violation
{
    int result; // INLINE COMMENT NOT A PROLOGUE, SHOULD FLAG VIOLATION
    result = a*b;
    return result; 
}

