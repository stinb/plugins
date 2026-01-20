// Custom

#include <stdint.h>

static void invalidFunc1(int a); // UndCC_Violation
static void invalidFunc1(int b)  // UndCC_Violation
{}

static void invalidFunc2(int);  // UndCC_Violation
static void invalidFunc2(int a) // UndCC_Violation
{}

static void invalidFunc3(int, int, double);      // UndCC_Violation
static void invalidFunc3(int a, int b, double c) // UndCC_Violation
{}

static void invalidFunc4(int a); // UndCC_Violation
static void invalidFunc4(int)    // UndCC_Violation
{}

typedef struct { int x; } a;

static void invalidFunc5(a);  // UndCC_Violation - stupid and invalid
static void invalidFunc5(a a) // UndCC_Violation - stupid and invalid
{}

static void invalidFunc6(a a); // UndCC_Violation - stupid and invalid
static void invalidFunc6(a)    // UndCC_Violation - stupid and invalid
{}

static void validFunc0(a a); // UndCC_Valid - stupid, but valid
static void validFunc0(a a)  // UndCC_Valid - stupid, but valid
{}

static void validFunc1(int a);
static void validFunc1(int a)
{}

static void validFunc2(int a, int b, double c);
static void validFunc2(int a, int b, double c)
{}

void validFunc3(int32_t a);
void validFunc3(signed int a) // UndCC_Valid - stupid, but valid
{}

void validFunc4(int, int); // UndCC_Valid
