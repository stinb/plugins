#include "test1.h"

class TempClass
{
  public:
    int invalidMember;                     // UndCC_Violation
    int32_t validMember;
};

struct TempStruct
{
  unsigned int validBitField : 4;
  unsigned int invalidNormalField;         // UndCC_Violation
};

int invalidReturnType()                    // UndCC_Violation
{
  const float64_t someFloat = 0.42;
  return (int) someFloat;
}

double alsoInvalidReturnType();            // UndCC_Violation

float128_t validReturnType();

int32_t alsoValidReturnType();

int32_t validFuncWithInvalidParameter(
  int invalidParam,                        // UndCC_Violation
  int64_t validParam);


int invalidArray[100];                     // UndCC_Violation
int *invalidPointer = invalidArray;        // UndCC_Violation
int invalidInt = 0;                        // UndCC_Violation
int32_t validInt = 100;
int64_t stillValidInt = 1000L;
long double invalidLongDouble = 100.00L;   // UndCC_Violation

int32_t main() {
  return 0;
}
