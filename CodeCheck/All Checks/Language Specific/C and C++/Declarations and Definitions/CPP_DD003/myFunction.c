#include <stdio.h>

int addNumbers(int a, int b);    // function prototype

int subNumbers(int a, int b)     // function defined with no explicit declaration (no prototype)    // UndCC_Violation
{
	int result;
	result = a-b;
	return result;
}

int main()
{
    int n1,n2,sum,diff;

    printf("Enters two numbers: ");
    scanf("%d %d",&n1,&n2);

    sum = addNumbers(n1, n2);        // function call
    printf("sum = %d",sum);
	
    diff = subNumbers(n1, n2);


    return 0;
}

int addNumbers(int a, int b)         // function definition   
{
    int result;
    result = a+b;
    return result;
}
