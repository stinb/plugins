#include <iostream>
using namespace std;

int addition (int a, int b)
{
  long dummy;  /* UndCC_Violation unused variable */
  int r;
  r=a+b;
  return r;
}

int main ()
{
  int z;
  z = addition (5,3);
  cout << "The result is " << z;
}
