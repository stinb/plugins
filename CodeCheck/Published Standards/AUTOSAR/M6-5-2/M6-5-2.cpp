// M6-5-2.cpp - sample source for M6-5-2
#include <cstdint>

void F1()
{
  int i;
  for ( i = 1; i != 10; i += 2 ) // UndCC_Violation
  {
    // ...
  }
  
  for ( i = 1; i <= 10; i += 2 ) // UndCC_Valid
  {
    // ...
  }
  
  for ( i = 1; i != 10; ++i ) // UndCC_Valid
  {
    // ...
  }
  
}

void F2()
{
  int i;
	int iMaxCtr = 10;
  
  i = 1;
  while ( i != 10 ) // UndCC_Violation
  {
    // ...
    i += 2;
  }
  
  i = 1;
  while (i <= 10) // UndCC_Valid
  {
    // ...
    if(true)
    {
      // ...
    }
    else
    {
      // ...
      if (true)
      {
        // ...
      }
    }
    i += 2;
  }
  
  iMaxCtr = iMaxCtr + 1;
  i = 1;
  while (iMaxCtr != i) // UndCC_Valid
  {
    // ...
    ++i;
  }
  
}

void F3()
{
  int i;
  
  i = 1;
  do
  {
    // ...
    i += 2;
  } while ( i != 10 ); // UndCC_Violation
  
  i = 1;
  do
  {
    // ...
    i += 2;
  }while (i <= 10); // UndCC_Valid
  
  i = 1;
  do
  {
    // ...
    ++i;
  }while (i != 10); // UndCC_Valid
  
}
