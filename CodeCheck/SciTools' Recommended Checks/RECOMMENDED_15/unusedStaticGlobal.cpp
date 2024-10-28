static int Okay1;
static int Okay2=1;
static int Okay3;
int Okay4; // not static

static int Violation1; // UndCC_Violation
static int Violation2=1; // UndCC_Violation
static int Violation3; // UndCC_Violation

void func1(int);
void func2() {
  Okay1=1;
  ++Okay2;
  Okay3;
  func1(Okay4);

#if 0
  Violation3=1;
#endif
}
