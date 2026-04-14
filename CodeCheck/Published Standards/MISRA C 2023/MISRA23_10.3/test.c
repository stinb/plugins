#include <limits.h>
#include <time.h>
#include <string.h>
#include <stddef.h>

void func(void) {
  unsigned long int u_a = ULONG_MAX;
  signed char sc;
  sc = (signed char)u_a; // UndCC_Violation - Cast eliminates warning
 
  
  unsigned long int u_a1 = ULONG_MAX;
  signed char sc1;
  if (u_a1 <= SCHAR_MAX) {
    sc1 = (signed char)u_a1;  // UndCC_Valid
  } else {
    /* Handle error */
  }
}

void func1(signed int si2) {
  /* Cast eliminates warning */
  unsigned int ui2 = (unsigned int)si2; // UndCC_Violation
  
  /* ... */
}

void func2(signed int si3) {
  unsigned int ui3;
  if (si3 < 0) {
    /* Handle error */
  } else {
    ui3 = (unsigned int)si3;  // UndCC_Valid
  }
 }
  
 void func3(void) {
   signed long int s_a4 = LONG_MAX;
   signed char sc4 = (signed char)s_a4; // UndCC_Violation
   /* ... */
}


void func4(void) {
  unsigned long int u_a5 = ULONG_MAX;
  unsigned char uc5;
  if (u_a5 > UCHAR_MAX) {
    /* Handle error */
  } else {
    uc5 = (unsigned char)u_a5; // UndCC_Valid
  }
  /* ... */
}

void func5(void) {
  time_t now = time(NULL);
  if (now != -1) {  // UndCC_FalseNeg
    /* Continue processing */
  }
}

void func6(void) {
  time_t now = time(NULL);
  if (now != (time_t)-1) { // UndCC_Valid
    /* Continue processing */
  }
}

/* Issue #4766 - Cast to void* should not trigger 10.3 */
void foo(void *p);
void func7(void) {
  char buf[2];
  foo((void*)&buf[0]);       // UndCC_Valid - pointer cast, not arithmetic
  char *p1 = &buf[0];       // UndCC_Valid
  void *p2 = (void*)&buf[0]; // UndCC_Valid - pointer cast, not arithmetic
}

