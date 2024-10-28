#include <stdio.h>
#define S(val) _Generic(val, int : 2, \
short : 3, \
default : 1)

  
  void func(void) {
  int a = 0;
  int b = S(a++); // UNDCC_Violation
  printf("%d, %d\n", a, b);
}


void func1(void) {
  int a = 14;
  int b = sizeof(a++); // UNDCC_Violation
  printf("%d, %d\n", a, b);
}


void func2(void) {
  int val = 0;
  /* ... */ 
  size_t align = _Alignof(int[++val]); // UNDCC_Violation
  printf("%zu, %d\n", align, val);
  /* ... */
}

// UndCC_Valid
void func3(void) {
  int a = 14;
  int b = sizeof(a);
  ++a;
  printf("%d, %d\n", a, b);
}

// UndCC_Valid
void func4(void) {
  int a = 0;
  int b = S(a);
  ++a;
  printf("%d, %d\n", a, b);
} 

// UndCC_Valid
void func5(void) {
  int val = 0; 
  /* ... */ 
  ++val;
  size_t align = _Alignof(int[val]);
  printf("%zu, %d\n", align, val);
  /* ... */
}
