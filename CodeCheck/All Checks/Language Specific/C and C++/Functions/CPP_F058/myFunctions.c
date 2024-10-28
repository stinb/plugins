#include <stdio.h>

/*
 * Definition for addNumbers().
 * 
 * 
 *
 */
int addNumbers(int a, int b) //UndCC_Violation
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


