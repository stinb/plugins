#include <stdio.h>


int main()
{ // UNDCC_Violation
return 0;
}

int func0()
{
 return 0; // UNDCC_Violation
}

int func1()
{
  return 1;
}

int func2()
{
   return 2; // UNDCC_Violation
}

int func3()
{
    return 3; // UNDCC_Violation
}

