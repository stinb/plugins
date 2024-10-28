#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <iostream>

int foo()
{
    int age;

    std::cout << "Enter your age: ";
    scanf("%d", &age); // UndCC_Violation

    return 0;
}

int foo_2()
{
    int i;
    char buffer[10];
    printf("Enter a number: ");
    fgets(buffer, 10, stdin);
    i = atoi(buffer); // UndCC_Violation
    return 0;
}
