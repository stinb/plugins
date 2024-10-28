static void invalidFunc1(int a); // UndCC_Violation
static void invalidFunc1(int b) { // UndCC_Violation
}

static void invalidFunc2(int); // UndCC_Violation
static void invalidFunc2(int a) { // UndCC_Violation
}

static void invalidFunc3(int, int, double); // UndCC_Violation
static void invalidFunc3(int a, int b, double c) { // UndCC_Violation
}

static void validFunc1(int a);
static void validFunc1(int a) {
}

static void validFunc2(int a, int b, double c);
static void validFunc2(int a, int b, double c) {
}

typedef signed   int    int32_t;
void validFunc3(int32_t a);
void validFunc3(signed int a) { /* stupid, but valid */
}
