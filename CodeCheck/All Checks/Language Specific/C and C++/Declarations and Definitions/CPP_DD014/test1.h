// See test2.c for other declarations

typedef int INT;
typedef void VOID;

// Objects - no extern keyword
int int0;
int int1 = 1;
int int2;
INT int3;
INT int4 = 4;
INT int5;
int int6;     // UndCC_Violation(a) - Type is INT in other declaration
int int7 = 4; // UndCC_Violation(a) - Type is INT in other declaration
int int8;     // UndCC_Violation(a) - Type is INT in other declaration

// Objects - extern keyword in this file
extern int int9;
extern int int10;
extern INT int11;
extern INT int12;
extern int int13; // UndCC_Violation(a) - Type is INT in other declaration
extern int int14; // UndCC_Violation(a) - Type is INT in other declaration

// Objects - comma-separated declaration, pointers
int **int16, int17;
int *int18, int19; // UndCC_Violation(a) - Type is INT in other declaration

// Objects - functional macro arguments
int int20; // UndCC_FalseNeg - Macro in another declaration prevents seeing custom type
int int21;
