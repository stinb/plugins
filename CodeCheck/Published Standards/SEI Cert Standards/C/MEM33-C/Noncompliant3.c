#include <stdio.h>
#include <stdlib.h>

struct flex_array_struct
{
    size_t num;
    int data[];
};

void print_array(struct flex_array_struct struct_p)
{
    puts("Array is: ");
    for (size_t i = 0; i < struct_p.num; ++i)
    {
        printf("%d ", struct_p.data[i]);        // UndCC_Violation
    }
    putchar('\n');
}

void func(void)
{
    struct flex_array_struct *struct_p;
    size_t array_size = 4;

    /* Space is allocated for the struct */
    struct_p = (struct flex_array_struct *)malloc(
        sizeof(struct flex_array_struct) + sizeof(int) * array_size);
    if (struct_p == NULL)
    {
        /* Handle error */
    }
    struct_p->num = array_size;

    for (size_t i = 0; i < array_size; ++i)
    {
        struct_p->data[i] = i;
    }
    print_array(*struct_p);
}
