#include <setjmp.h> // UndCC_Violation


void fn_goto()
{
label:
  goto label; // UndCC_Violation
}


void direct_recursion()
{
  direct_recursion(); // UndCC_Violation
}


void indirect_recursion_0();
void indirect_recursion_1();
void indirect_recursion_2();

void indirect_recursion_0()
{
  indirect_recursion_1(); // UndCC_Violation
}

void indirect_recursion_1()
{
  indirect_recursion_2(); // UndCC_Violation
}

void indirect_recursion_2()
{
  indirect_recursion_0(); // UndCC_Violation
}
