// Place & and * modifiers adjacent to variable name.

int *Okay1;
int **Okay2;
int &Okay3=/*this is fine*/*Okay1;
int *Okay4();
int **Okay5() {}
typedef int *Okay6;
int func1(
  int *Okay7,
  int &Okay8)
{
  int *Okay9;
}

int * Violation1; // UndCC_Violation
int * *Violation2; // UndCC_Violation
int & Violation3/*this is fine*/=*Violation1; // UndCC_Violation
int * Violation4(); // UndCC_Violation
int * *Violation5() {} // UndCC_Violation
typedef int * Violation6; // UndCC_Violation
int/*this is fine*/ func2(
  int * Violation7, // UndCC_Violation
  int & Violation8) // UndCC_Violation
{
  int * Violation9; // UndCC_Violation
}
