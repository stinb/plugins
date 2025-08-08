#include <stdint.h>

/* Value object containing an array as an element */
struct S1
{
    int32_t array[10];
};

struct S1 s1;
struct S1 getS1(void);

void foo(int32_t const *p);

int main()
{
    /* Compliant - not temporary storage duration */
    int32_t *p = s1.array;
    s1.array[0] = 1;
    foo(s1.array);

    /* Non-compliant - temporary storage duration */
    p = getS1().array;    /* UndCC_Violation , also creates dangling pointer        */
    foo(getS1().array);   // UndCC_Violation
    foo((s1 = s1).array); /* UndCC_Violation , other forms of non-lvalue expression */

    /* Compliant - immediate element access is always safe */
    int32_t j = getS1().array[3]; /* element copied: const access         */
    j = (s1 = s1).array[3];

    /* Non-compliant - element used as a modifiable lvalue */
    getS1().array[3] = j;       // UndCC_Violation
    (1 ? s1 : s1).array[3] = j; // UndCC_Violation

    union U
    {
        int arr[5];
    };

    union U getU(void);

    int *u = getU().arr; // UndCC_Violation
}
