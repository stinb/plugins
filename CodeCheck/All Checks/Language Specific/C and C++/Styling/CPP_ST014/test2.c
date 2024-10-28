#include <stdio.h>

int main()
{
    int a = 5, b = 5, c = 10;
    
        printf("This line will not flag a violation if max columns are set to 80 \n"); //UndCC_Valid Ignores leading whitespace.




    printf("This line of code will default a violation because it is longer than 80 columns in length \n"); //UndCC_Violation

    return 0;
}
