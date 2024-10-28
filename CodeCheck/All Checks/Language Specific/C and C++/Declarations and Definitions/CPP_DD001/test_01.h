#include <stdio.h>

// declare some globals
int a;
static int b;
int c;

void add(int x, int y) 
{ 
  c++;                                          // flags violation, should not be global // UndCC_Violation
  printf("Added value=%d\n", x + y); 
} 
  
void multiply(int z, int k) 
{ 
  b = 4;                                       // correctly made static, will not flag violation
  printf("Multiplied value=%d\n", z * k); 
}

