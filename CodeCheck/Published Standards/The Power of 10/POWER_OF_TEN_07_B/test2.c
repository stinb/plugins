#include <stdio.h>

int ACCESS[5] = {1, 2, 3, 4, 5};

struct person
{
    char name[50];
    int age;
};

void foo(struct person *p) // UndCC_Violation - pointer parameter not checked for null
{
    if (p->age >= 60)
        printf("Person is senior");

    //...
}

void foo2(struct person *p)
{
    if (p == NULL)
    {
        printf("Error");
        return;
    }

    if (p->age >= 60)
        printf("Person is senior");

    //...
}

int foo3(int x) // UndCC_Violation - unchecked parameter
{
    return ACCESS[x];
}

int foo4(int x)
{
    if ((x < 0) || (x > 5))
        return -1;

    return ACCESS[x];
}

void foo5(int x) // UndCC_Violation - unchecked parameter
{
    int ret;

    ret = foo4(x); // UndCC_Violation - unchecked return value

    printf("%d is a positive number.", ret);
}

void foo6(int x) // UndCC_Violation - unchecked parameter
{
    int ret;

    ret = foo4(x);

    if (ret < 0)
        printf("%d is a negative number.", ret);

    printf("%d is a positive number.", ret);
}
