#include "test.h"

/* Defining declarations - compliant */
int32_t i = 0;                        // UndCC_Valid - defining declaration
int32_t arr[10] = {0};                // UndCC_Valid - defining declaration with initializer
extern int32_t init_val = 42;         // UndCC_Valid - extern with initializer is a definition

/* Extern object declarations - non-compliant */
extern int32_t j;                     // UndCC_Violation - extern object in source file
extern const int32_t k;               // UndCC_Violation - extern const object
extern volatile int32_t v;            // UndCC_Violation - extern volatile object
extern int32_t *ptr;                  // UndCC_Violation - extern pointer

/* Extern function declarations - non-compliant */
extern void foo(void);                // UndCC_Violation - extern function in source file
extern int32_t compute(int32_t a);    // UndCC_Violation - extern function with return type
void bar(int32_t x, int32_t y);       // UndCC_Violation - implicit extern function declaration

/* Static declarations - compliant (internal linkage) */
static int32_t s;                     // UndCC_Valid - internal linkage
static int32_t s2 = 10;              // UndCC_Valid - static with initializer
static void helper(void);            // UndCC_Valid - static function declaration
static int32_t static_func(int32_t); // UndCC_Valid - static function with params

/* Function definitions - compliant */
void worker(bool flag)                // UndCC_Valid - function definition
{
    (void)flag;
}

int32_t adder(int32_t a, int32_t b)   // UndCC_Valid - function definition
{
    return a + b;
}

/* Block scope extern declarations - non-compliant */
void uses_externs(void)
{
    extern bool flag;                 // UndCC_Violation - block scope extern object
    extern void baz(void);            // UndCC_Violation - block scope extern function
    extern int32_t counter;           // UndCC_Violation - block scope extern object
    (void)flag;
}

/* Nested block scope extern - non-compliant */
void nested_externs(void)
{
    if (1)
    {
        extern int32_t deep;         // UndCC_Violation - nested block scope extern
        (void)deep;
    }
}

/* Function with only local variables - compliant */
void local_vars(void)
{
    int32_t local_a = 0;             // UndCC_Valid - local variable
    int32_t local_b = 1;             // UndCC_Valid - local variable
    (void)local_a;
    (void)local_b;
}

static void helper(void)             // UndCC_Valid - static function definition
{
}

static int32_t static_func(int32_t x) // UndCC_Valid - static function definition
{
    return x;
}
