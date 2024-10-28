// Test: declaration of types or externals at the function level

typedef int Okay1;
struct Okay2;
struct Okay3 {};
extern void okay4();
void okay5();
extern int okay6;

void func1() {
	int okay7;
  typedef int Violation1; // UndCC_Violation
  struct Violation2; // UndCC_Violation
  struct Violation3 {}; // UndCC_Violation
  extern void violation4(); // UndCC_Violation
  void violation5(); // UndCC_Violation
  extern int violation6; // UndCC_Violation
}
