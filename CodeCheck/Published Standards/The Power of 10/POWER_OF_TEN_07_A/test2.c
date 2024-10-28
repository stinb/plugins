#include <stdio.h>

int ACCESS[5] = {1, 2, 3, 4, 5};

struct person
{
    char name[50];
    int age;
};

void foo(struct person *p)
{
    if (p->age >= 60)
        printf("Person is senior"); // UndCC_Violation - result ignored

    //...
}

void foo2(struct person *p)
{
    if (p == NULL)
    {
        printf("Error");            // UndCC_Violation - result ignored
        return;
    }

    if (p->age >= 60)
        printf("Person is senior"); // UndCC_Violation - result ignored

    //...
}

int foo3(int x)
{
    return ACCESS[x];
}

int foo4(int x)
{
    if ((x < 0) || (x > 5))
        return -1;

    return ACCESS[x];
}

void foo5(int x)
{
    int ret;

    ret = foo4(x);

    printf("%d is a positive number.", ret); // UndCC_Violation - result ignored
}

void foo6(int x)
{
    int ret;

    ret = foo4(x);

    if (ret < 0)
        printf("%d is a negative number.", ret); // UndCC_Violation - result ignored

    printf("%d is a positive number.", ret);     // UndCC_Violation - result ignored
}
