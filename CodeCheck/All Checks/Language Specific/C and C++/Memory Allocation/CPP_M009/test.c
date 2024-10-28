#include <stdlib.h>
int main()
{
  int* ptr;
  int * a = ptr;
  int n, i;

  
  ptr = (int*)malloc(n * sizeof(int));
  for (i = 0; i < n; ++i) {
    ptr[i] = i + 1;
  }

  int c = ptr[0]; // UndCC_Valid
  
  free(ptr);
  int d = ptr[0]; // UndCC_Violation
  
  return 0;
}
