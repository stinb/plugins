#include "test1.h"

// Objects - no extern keyword
int int0;
int int1;
int int2 = 2;
INT int3;
INT int4;
INT int5 = 5;
INT int6;     // UndCC_Violation(a) - Type is int in other declaration
INT int7;     // UndCC_Violation(a) - Type is int in other declaration
INT int8 = 8; // UndCC_Violation(a) - Type is int in other declaration

// Objects - extern keyword in other file
int int9;
int int10 = 10;
INT int11;
INT int12 = 12;
INT int13;      // UndCC_Violation(a) - Type is int in other declaration
INT int14 = 12; // UndCC_Violation(a) - Type is int in other declaration

// Objects - comma-separated declaration, pointers
int int17, **int16;
INT int19, *int18; // UndCC_Violation(a) - Type is int in other declaration

// Objects - functional macro arguments
#define INT_var(identifier) INT identifier
#define int_var(identifier) int identifier
INT_var(int20); // UndCC_FalseNeg - Macro in this declaration prevents seeing custom type
int_var(int21);

// Functions - functional macro arguments
#define VOID_fn(identifier) VOID identifier (VOID)
#define void_fn(identifier) void identifier (void)
VOID_fn(fn0) { } // UndCC_FalseNeg - Macro in this declaration prevents seeing custom type
void fn0(void);  // UndCC_FalseNeg - Macro in above declaration
void_fn(fn1) { }
void fn1(void);
