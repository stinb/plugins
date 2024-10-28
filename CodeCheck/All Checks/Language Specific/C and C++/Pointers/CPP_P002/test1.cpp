// Test: Pointer must be initialized to 0, or some valid
// value when created.

int *okay1A = 0;
int *okay1B = okay1A;

int *violation1A = NULL; // UndCC_Violation
int *violation1B = 1; // UndCC_Violation
int *violation1C = 0x0; // UndCC_Violation
