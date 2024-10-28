enum MyEnumType { ALPHA, BETA, GAMMA };

union mytypes_t {
  char c;
  int i;
  float f;
} mytypes;

void f1 ( )
{
  class TYPE { };
}
void f2 ( )
{
  float TYPE; // UndCC_Violation
}

void f3 ()
{
  int MyEnumType = 1;  // UndCC_Violation
}
