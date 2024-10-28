#include <stdio.h>
#include <stdlib.h>

struct flex_array_struct
{
    size_t num;
    int data[];
};

void print_array(struct flex_array_struct *struct_p)
{
    puts("Array is: ");
    for (size_t i = 0; i < struct_p->num; ++i)
    {
        printf("%d ", struct_p->data[i]);
    }
    putchar('\n');
}

void func(void)
{
    struct flex_array_struct *struct_p;
    size_t array_size = 4;

    /* Space is allocated for the struct and initialized... */

    print_array(struct_p);
}