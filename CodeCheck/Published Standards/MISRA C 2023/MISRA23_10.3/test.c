#include <limits.h>
#include <time.h>
#include <string.h>
#include <stddef.h>

void func(void) {
  unsigned long int u_a = ULONG_MAX;
  signed char sc;
  sc = (signed char)u_a; // UndCC_Valid - cast gives the same essential type
 
  
  unsigned long int u_a1 = ULONG_MAX;
  signed char sc1;
  if (u_a1 <= SCHAR_MAX) {
    sc1 = (signed char)u_a1;  // UndCC_Valid
  } else {
    /* Handle error */
  }
}

void func1(signed int si2) {
  unsigned int ui2 = (unsigned int)si2; // UndCC_Valid - cast gives the same essential type
  
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
   signed char sc4 = (signed char)s_a4; // UndCC_Valid - cast gives the same essential type
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


/* A cast sets the essential type of the expression assigned */
typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef unsigned int u32_t;

struct leap_sec_t {
  int8_t   pending_offset;
  uint32_t pending_offset_abs;
};

uint8_t dataArr[8];

void func8(void) {
  struct leap_sec_t leap_sec = {0};
  int32_t state = 2;
  uint8_t val2;
  uint32_t inWord1 = 123345U;
  uint32_t inWord2 = 4321U;

  dataArr[0] = (uint8_t)leap_sec.pending_offset;        // UndCC_Valid - cast, through a member
  dataArr[2] = (uint8_t)(leap_sec.pending_offset_abs >> 8); // UndCC_Valid - cast of a composite
  int32_t outWord = (int32_t)(inWord1 - inWord2);       // UndCC_Valid for 10.3 - cast, but violates 10.8
  val2 = dataArr[state] & 0x80U;                        // UndCC_Valid - 0x80U is essentially unsigned char

  dataArr[1] = leap_sec.pending_offset;                 // UndCC_Violation - signed to unsigned, no cast
  val2 = leap_sec.pending_offset_abs;                   // UndCC_Violation - narrower essential type
}

enum colour { RED, GREEN };

void func9(int32_t s32, uint32_t u32, char *p, uint8_t *up, int cond) {
  u32_t   ut;
  uint8_t u8 = 0U;
  int8_t  s8 = 0;
  char    ch = 'a';
  int32_t i32;
  enum colour col;

  ut = (u32_t)s32;              // UndCC_Valid - cast through a typedef of a base type
  u32 = 0x1E5;                  // UndCC_Valid - a hex constant is not floating
  u32 = 0xFFFFFFFF;             // UndCC_Valid - an unsuffixed hex constant that does not fit int is unsigned
  u8 >>= 1;                     // UndCC_Valid - a shift keeps the essential type of its left operand
  i32 = u8 + s8;                // UndCC_Valid - both operands promote to signed int
  i32 = ch - 'a';               // UndCC_Valid - character arithmetic takes the standard type
  ch = 'a' + 1;                 // UndCC_Valid - character plus signed keeps character
  i32 = p[1] - '0';             // UndCC_Valid - subscript of a pointer
  col = cond ? RED : GREEN;     // UndCC_Valid - a conditional takes the type of its last two operands
  if (p != 0)
    *p++ = '\0';                // UndCC_Valid - the condition is not part of the target

  switch (cond) {
    case 1:
      ch = 'x';                 // UndCC_Valid - a case label is not part of the target
      break;
    default:
      break;
  }

  i32 = s32 - 1U;               // UndCC_Violation - the constant is an unsigned int here
  *up = 'a';                    // UndCC_Violation - character through a pointer to unsigned
  s8 = ~s8;                     // UndCC_Violation - a signed complement takes the standard type
}
