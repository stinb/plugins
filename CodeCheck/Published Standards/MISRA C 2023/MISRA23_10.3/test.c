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

#include <stdbool.h>

void booleanConstants(void)
{
  bool b1 = false;              // UndCC_Valid - false from stdbool.h is essentially Boolean
  bool b2 = true;               // UndCC_Valid
  bool *bp = &b1;
  int i;
  b1 = true;                    // UndCC_Valid
  *bp = false;                  // UndCC_Valid
  i = true;                     // UndCC_Violation - Boolean assigned to a signed object
  b2 = 1;                       // UndCC_Violation - a signed constant is not essentially Boolean
  (void)b2; (void)i;
}

void unaryMinus(void)
{
  int i32;
  signed char s8;
  short s16;
  unsigned int u32;
  unsigned char u8 = 1U;
  long long s64;
  i32 = -2147483648;            // UndCC_Valid - a negated constant takes the narrowest signed type of its value
  s8 = -128;                    // UndCC_Valid
  s8 = -129;                    // UndCC_Violation - needs 16 bits
  s16 = -32768;                 // UndCC_Valid
  s64 = -2147483649;            // UndCC_Valid
  i32 = -2147483649;            // UndCC_Violation - needs 64 bits
  u32 = -1U;                    // UndCC_Valid - negating unsigned int gives its standard type, unsigned int
  i32 = -1U;                    // UndCC_Violation - unsigned assigned to signed
  i32 = -u8;                    // UndCC_Valid - an unsigned char operand gives the standard type int
  (void)i32; (void)s8; (void)s16; (void)u32; (void)s64;
}

void takesU32(unsigned int u);
void takesS32(int s);
void takesU8(unsigned char u);
void takesVariable(int n, ...);
int  passThrough(int s);
#define CALL_U32(x) takesU32(x)

unsigned char narrowReturn(unsigned short x)
{
  return x;                     // UndCC_Violation - uint16_t to uint8_t
}

unsigned char sameReturn(unsigned char x)
{
  return x;                     // UndCC_Valid
}

unsigned int constantReturn(void)
{
  return 5;                     // UndCC_Valid - by Exception 1
}

int booleanReturn(int a, int b)
{
  return a > b;                 // UndCC_Violation - essentially Boolean returned as signed
}

void callArguments(void)
{
  int s = 5;
  unsigned short u16 = 1U;
  takesU32(s);                  // UndCC_Violation - signed passed to unsigned
  takesU32(5);                  // UndCC_Valid - by Exception 1
  takesU32(-1);                 // UndCC_Violation - the constant cannot be represented
  takesS32(0xFFFFFFFE);         // UndCC_Violation - unsigned passed to signed
  takesS32(-2147483648);        // UndCC_Valid
  takesU8(u16);                 // UndCC_Violation - narrower
  takesS32(s + 1);              // UndCC_Valid
  takesVariable(1, u16, s);     // UndCC_Valid - a variable argument list has no parameter type
  takesU32(passThrough(s));     // UndCC_Violation - the call returns signed
  takesS32(passThrough(u16));   // UndCC_Violation - the inner argument is unsigned
  CALL_U32(s);                  // UndCC_Valid - calls inside a macro are not checked
}

void libraryArguments(char *d, const char *s, int n)
{
  memcpy(d, s, n);              // UndCC_Violation - signed passed to size_t
  memcpy(d, s, 4);              // UndCC_Valid - by Exception 1
  memset(d, 0, sizeof(int));    // UndCC_Valid
  (void)strncmp(d, s, n);       // UndCC_Violation
}

void parenthesizedOperand(int s)
{
  unsigned int u;
  u = (s - 1) * s;              // UndCC_Violation - signed assigned to unsigned
  u = (unsigned int)s * 2U;     // UndCC_Valid - the cast gives unsigned
}
