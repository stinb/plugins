#include <stdint.h>

/* Basic tentative definitions */
int32_t a;                              // UndCC_Violation
static int32_t b;                       // UndCC_Violation
int16_t c;                              // UndCC_Violation
unsigned int d;                         // UndCC_Violation
const int32_t e;                        // UndCC_Violation - const without initializer

/* Extern declarations */
extern int32_t f;                       // UndCC_Valid
extern int16_t g;                       // UndCC_Valid
extern const int32_t h;                 // UndCC_Valid

/* Definitions with initializers */
int32_t i = 0;                          // UndCC_Valid
static int32_t j = 1;                   // UndCC_Valid
const int32_t k = 42;                   // UndCC_Valid
int16_t l = -1;                         // UndCC_Valid
unsigned int m = 100u;                  // UndCC_Valid

/* Pointer types */
int32_t *p;                             // UndCC_Violation
static char *q;                         // UndCC_Violation
extern int32_t *r;                      // UndCC_Valid
int32_t *s = 0;                         // UndCC_Valid

/* Array types */
int32_t arr[10];                        // UndCC_Violation
static char buf[256];                   // UndCC_Violation
extern int32_t ext_arr[10];             // UndCC_Valid

/* Function declarations - not objects */
void func1(void);                       // UndCC_Valid
int32_t func2(int32_t x, int32_t y);   // UndCC_Valid
static int32_t func3(void);             // UndCC_Valid
extern void func4(void);               // UndCC_Valid

/* Typedefs and structs - not objects */
typedef int32_t my_type;                // UndCC_Valid
struct my_struct { int32_t x; };        // UndCC_Valid
