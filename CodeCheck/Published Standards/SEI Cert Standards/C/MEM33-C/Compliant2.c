#include <string.h>

struct flex_array_struct
{
    size_t num;
    int data[];
};

void func(struct flex_array_struct *struct_a,
          struct flex_array_struct *struct_b)
{
    if (struct_a->num > struct_b->num)
    {
        /* Insufficient space; handle error */
        return;
    }
    memcpy(struct_b, struct_a,
           sizeof(struct flex_array_struct) + (sizeof(int) * struct_a->num));
}