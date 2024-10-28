// Custom

#include <iostream>

int main()
{

  int arr1[10];   /* UndCC_Valid */
  arr1[0] = 2;

  int n = 10;
  int arr2[n];    /* UndCC_Violation - identifier used to size array */
  arr2[0] = 2;

  int arr3[6] = { 10, 20, 30, 40 };    /* UndCC_Valid */

  printf("%d %d %d", arr1[0], arr2[0], arr3[0]);

  return 0;
}
