#include <stddef.h>

struct flex_array_struct2
{
    size_t num;
    int data[];
};

void func(struct flex_array_struct2 *struct_a,
          struct flex_array_struct2 *struct_b,
	  struct flex_array_struct2 *temp)
{
    *temp = *struct_a;      // UndCC_Violation
    *struct_b = *temp;      // UndCC_Violation
}
