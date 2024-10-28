#include <float.h>
#include <math.h>
#include <stdio.h>



void f(void) {
  if (sizeof(int) == sizeof(float)) {
    float f = 0.0f;
    int *ip = (int *)&f; // UNDCC_Violation
    (*ip)++;
    printf("float is %f\n", f);
  }
}

// UndCC_Valid
void f1(void) {
  float f = 0.0f;
  f = nextafterf(f, FLT_MAX);
  printf("float is %f\n", f);
}


void func(void) {
  short a[2];
  a[0]=0x1111;
  a[1]=0x1111;
  
  *(int *)a = 0x22222222; // UNDCC_Violation
  
  printf("%x %x\n", a[0], a[1]);
}

// UndCC_Valid
void func1(void) {
  union {
    short a[2];
    int i;
  } u;
  
  u.a[0]=0x1111;
  u.a[1]=0x1111;
  u.i = 0x22222222;
  
  printf("%x %x\n", u.a[0], u.a[1]);
  
  /* ... */
}
