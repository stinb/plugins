#include <stdio.h>

int main()
{
    int a = 5, b = 5, c = 10;

    printf("This line of code will default a violation because it is longer than 80 columns in length \n"); // UndCC_Violation

    printf("This line will flag a violation if max columns set to 70 \n");

    return 0;
}
