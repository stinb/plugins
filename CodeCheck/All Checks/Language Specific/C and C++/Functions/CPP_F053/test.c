#include <stdio.h>

int func() // UndCC_Violation
{
}

void func1()
{
}

func2() // UndCC_Violation
{
  return 3;
}

int main()
{
    return 0;
}


