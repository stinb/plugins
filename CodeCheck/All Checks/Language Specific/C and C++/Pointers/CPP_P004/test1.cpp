// Test: Pointers must be initialized to 0, or some valid value when created.

int okay1A;             // OKAY: not pointer
int *okay1B=0;          // OKAY: init

class Class1 {
  int *okay1C;          // OKAY: non-static member is ignored.
  static int okay1D;
  static int *okay1E;
};
int Class1::okay1D;     // OKAY: not pointer
int *Class1::okay1E=0;  // OKAY: init

void func1(
  int *p1)              // OKAY: parameters are not considered
{
  int okay1F;           // OKAY: not pointer
  int *okay1G=0;        // OKAY: init
}


int *violation1A;       // UndCC_Violation: no init

class Class2 {
  static int *violation1B;
};
int *Class2::violation1B;// UndCC_Violation: no init

void func2()
{
  int *violation1C;     // UndCC_Violation: no init
  int *violation1D;     // UndCC_Violation: no init
  violation1D=0;        // set ref doesn't count as init
}
