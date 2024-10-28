#include <stdio.h> 

inline void swap(int *m, int *n) // UndCC_Violation
{
  int tmp = *m;
  *m = *n;
  *n = tmp;
}

static inline int foo() 
{ 
  return 100; 
} 

// Driver code 
int main() 
{ 
  
  int ret; 
  
  // inline function call 
  ret = foo(); 
  
  printf("Output is: %d\n", ret); 
  return 0; 
} 
