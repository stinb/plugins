#include "test1.h"

// Basic numerical type declarations
signed char            a; // UndCC_Violation
unsigned char          b; // UndCC_Violation
short                  c; // UndCC_Violation
short int              d; // UndCC_Violation
signed short           e; // UndCC_Violation
signed short int       f; // UndCC_Violation
unsigned short         g; // UndCC_Violation
unsigned short int     h; // UndCC_Violation
int                    i; // UndCC_Violation
signed                 j; // UndCC_Violation
signed int             k; // UndCC_Violation
unsigned               l; // UndCC_Violation
unsigned int           m; // UndCC_Violation
long                   n; // UndCC_Violation
long int               o; // UndCC_Violation
signed long            p; // UndCC_Violation
signed long int        q; // UndCC_Violation
unsigned long          r; // UndCC_Violation
unsigned long int      s; // UndCC_Violation
long long              t; // UndCC_Violation
long long int          u; // UndCC_Violation
signed long long       v; // UndCC_Violation
signed long long int   w; // UndCC_Violation
unsigned long long     x; // UndCC_Violation
unsigned long long int y; // UndCC_Violation

// Structs and unions
struct Struct
{
  uint32_t a; // UndCC_Valid
  unsigned b; // UndCC_Violation
};
union Union
{
  uint32_t a; // UndCC_Valid
  unsigned b; // UndCC_Violation
};
struct BitFields
{
  // Spacing & comments
  int a:32;        // UndCC_Valid
  int b : /**/ 32; // UndCC_Valid

  // Numerical bases
  int c:0b10000;   // UndCC_Valid
  int d:020;       // UndCC_Valid
  int e:0x10;      // UndCC_Valid

  // Suffixes
  int g:0b1000u;   // UndCC_Valid
  int h:010U;      // UndCC_Valid
  int f:8l;        // UndCC_Valid
  int i:0x8L;      // UndCC_Valid
};

// No digits in typedef
typedef signed   char   intEight_t;            // UndCC_Violation
typedef signed   short  intSixteen_t;          // UndCC_Violation
typedef signed   int    intThirtyTwo_t;        // UndCC_Violation
typedef signed   long   intSixtyFour_t;        // UndCC_Violation
typedef unsigned char   uintEight_t;           // UndCC_Violation
typedef unsigned short  uintSixteen_t;         // UndCC_Violation
typedef unsigned int    uintThirtyTwo_t;       // UndCC_Violation
typedef unsigned long   uintSixtyFour_t;       // UndCC_Violation
typedef          float  floatThirtyTwo_t;      // UndCC_Violation
typedef          double floatSixtyFour_t;      // UndCC_Violation
typedef long     double floatOneTwentyEight_t; // UndCC_Violation

// Functions
void fn0();                 // UndCC_Valid
int  fn1();                 // UndCC_Violation
int *fn2();                 // UndCC_Violation
void fn3(int32_t x, int y); // UndCC_Violation

// Arrays and pointers
int32_t arr0[100]; // UndCC_Valid
int32_t *ptr0;     // UndCC_Valid
int arr1[100]; // UndCC_Violation
int *ptr1;     // UndCC_Violation

// MISRA C++ 2008 Exceptions
// wchar_t
typedef char wchar_t; // UndCC_Valid
wchar_t char0;        // UndCC_Valid
// char_t
char_t  char1;        // UndCC_Valid

// MISRA C 2012 Exceptions
// 1: Basic numerical types in typedefs with the number in the name
typedef float float32_t;             // UndCC_Valid
// 2-4: Basic numerical types in main function signature
int main( int argc, char *argv[] ); // UndCC_Valid
