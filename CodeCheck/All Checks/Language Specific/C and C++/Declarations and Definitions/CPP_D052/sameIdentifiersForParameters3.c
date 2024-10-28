static void invalidFunc1(int a);
static void invalidFunc1(int b);                  // UNDCC_Violation
static void invalidFunc1(int c);                  // UNDCC_Violation

static void validFunc1(int);
static void validFunc1(int a);                    // UNDCC_Valid

static void validFunc2(int a, int b, double c);
static void validFunc2(int, int, double);         // UNDCC_Valid

static void validFunc3(int a);
static void validFunc3(int a);                    // UNDCC_Valid

static void validFunc4(int a, int b, double c);
static void validFunc4(int a, int b, double c);   // UNDCC_Valid

typedef signed int int32_t;
void validFunc5(int32_t a);
void validFunc5(signed int a);                    // UNDCC_Valid
