#include <stdio.h>

const char *p;
void dont_do_this(void)
{
    const char c_str[] = "This will change";
    p = c_str; // UNDCC_Violation, c_str to go out of scope while p holds its address
}

void innocuous(void)
{
    printf("%s\n", p);
}

int main(void)
{
    dont_do_this();
    innocuous();
    return 0;
}

char *init_array(void)
{
    char array[10];
    /* Initialize array */
    return array; // Warning with -Wreturn-stack-address
}

void squirrel_away(char **ptr_param)
{
    char local[10];
    /* Initialize array */
    *ptr_param = local; // UNDCC_Violation, stores a pointer to local variable local into a location pointed to by function parameter ptr_param
}

void rodent(void)
{
    char *ptr;
    squirrel_away(&ptr);
    /* ptr is live but invalid here */
}
