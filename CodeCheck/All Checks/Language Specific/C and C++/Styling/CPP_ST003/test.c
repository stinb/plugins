#include <stdio.h>

int func() {    // UNDCC_Valid
}

int func1()
{    // UNDCC_Violation - inconsistent placement of braces
}

int func2() { }  // UNDCC_Violation - inconsistent placement of braces

int main()
  {    // UNDCC_Violation - inconsistent placement of braces

  }



