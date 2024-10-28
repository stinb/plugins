#include <stddef.h>

struct flex_array_struct
{
    size_t num;
    int data[];
};

void func(struct flex_array_struct *struct_a,
          struct flex_array_struct *struct_b)
{
    *struct_b = *struct_a;      // UndCC_Violation
}
