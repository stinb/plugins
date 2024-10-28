#include <stdio.h>
#include <iostream>
using namespace std;

void duplicate(int& b){  // UndCC_Violation - parameter b passed by reference
  b *= 2;
}

void swapnum(int &i, int &j) {  // UndCC_Violation - parameters i and j both passed by reference
  int temp = i;
  i = j;
  j = temp;
}

void swapnum_passbypointer(int *x, int *y)  // UndCC_Valid - parameters x and y passed by pointer
{
  int z = *x;
  *x = *y;
  *y = z;
}

int main(void) {
  // pass by reference driver code
  int a = 10;
  int b = 20;

  swapnum(a, b);
  printf("A is %d and B is %d\n", a, b);

  int x = 25;
  cout << "The original value of x is " << x << endl;
  duplicate(x);
  cout << "The new value of x is " << x;

  // pass by pointer driver code
  int c = 45, d = 35;
  cout << "Before Swap\n";
  cout << "a = " << c << " b = " << d << "\n";

  swapnum_passbypointer(&c, &d);

  cout << "After Swap with pass by pointer\n";
  cout << "a = " << c << " b = " << d << "\n";

  return 0;
}
