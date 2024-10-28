// Test: Pointers must be initialized to 0, or some valid value when created.

typedef int *PtrType1;
typedef PtrType1 PtrType2;

PtrType1 okay2A=0;
PtrType2 okay2B=0;

PtrType1 violation2A; // UndCC_Violation
PtrType2 violation2B; // UndCC_Violation
