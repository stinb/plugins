typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

#define NUMBER (8U)
#define LIMIT  5
#define ADD_ONE(x) ((x) + 1)

void casts(void)
{
  int32_t n;
  uint32_t bitRemaining = 17U;
  int32_t temp = 17000;
  uint32_t scale = 2000U;
  int32_t noise = 1234;
  n = (int32_t)bitRemaining - 1;            // UndCC_Valid - the cast gives signed
  n = (int32_t)scale * noise;               // UndCC_Valid
  scale = ((uint32_t)temp + 128U) >> 8;     // UndCC_Valid
  n = (int32_t)scale * ((int32_t)scale - 1048576); // UndCC_Valid
  n = bitRemaining - 1;                     // UndCC_Violation - unsigned and signed
}

void floating(void)
{
  double rate = 2.0;
  float f = 1.0f;
  rate = rate + 1.0;                        // UndCC_Valid - both floating
  rate = 1048576.0 / rate;                  // UndCC_Valid
  rate = rate + f;                          // UndCC_Valid
  rate = rate + 1;                          // UndCC_Violation - floating and signed
}

void macros(void)
{
  int32_t p;
  uint32_t q = NUMBER;
  if (p < NUMBER) {}                        // UndCC_Violation - NUMBER is unsigned
  if (q < NUMBER) {}                        // UndCC_Valid
  if (p < LIMIT) {}                         // UndCC_Valid
  p = ADD_ONE(p);                           // UndCC_Valid - operators in a macro body are not checked
}

void pointers(void)
{
  int32_t array[64];
  int32_t *ptr1 = &array[3];
  int32_t *ptr2 = &array[17];
  uint32_t r;
  int32_t d;
  r = (ptr2 - ptr1) + 1U;                   // UndCC_Violation - a pointer difference is signed
  r = ptr2 - ptr1 + 1U;                     // UndCC_Violation
  d = ptr2 - ptr1 + 1;                      // UndCC_Valid
  ptr1 = ptr1 + 1U;                         // UndCC_Valid - pointer arithmetic
  if (ptr1 == 0) {}                         // UndCC_Valid
}

void precedence(int32_t s, uint32_t u)
{
  int32_t r;
  r = s + s * s;                            // UndCC_Valid
  r = s - s - s;                            // UndCC_Valid
  r = -s * s;                               // UndCC_Valid - a prefix minus is not binary
  r = s * -s;                               // UndCC_Valid
  r = s + (int32_t)u * s;                   // UndCC_Valid
  if (s + 1 < (int32_t)u) {}                // UndCC_Valid
  if (s < u) {}                             // UndCC_Violation
  r = s * (int32_t)u + u;                   // UndCC_Violation - signed and unsigned at the +
  r = (s > 0) ? s : u;                      // UndCC_Violation - second and third operands differ
  r = (s > 0) ? s : 1;                      // UndCC_Valid
  u = u & 0xFFU;                            // UndCC_Valid
  u = u & 0xFF;                             // UndCC_Violation
  u *= s;                                   // UndCC_Violation - compound assignment
  u |= 1U;                                  // UndCC_Valid
}

void characters(char c, uint8_t u8)
{
  char d;
  d = c + 1;                                // UndCC_Valid - by exception
  d = 1 + c;                                // UndCC_Valid - by exception
  d = c - 1;                                // UndCC_Valid - by exception
  d = 1 - c;                                // UndCC_Violation - only a character left operand is excepted
  d = c * 2;                                // UndCC_Violation
  if (c == u8) {}                           // UndCC_Violation
  (void)d;
}

#if LIMIT > 0
int declarator(int *p, int32_t s)
{
  return *p + s;                            // UndCC_Valid
}
#endif

void arrays(int32_t i, uint32_t u)
{
  uint8_t buf[10];
  int32_t ibuf[10];
  uint8_t *p;
  int32_t *q;
  p = buf + i;                              // UndCC_Valid - an array is converted to a pointer
  q = ibuf + u;                             // UndCC_Valid
  if (buf[0] + i) {}                        // UndCC_Violation - the element is unsigned
  u = sizeof(buf) - 1U;                     // UndCC_Valid
  u = sizeof(buf) - 1;                      // UndCC_Violation - sizeof gives unsigned
}

void castThenPrefix(int32_t a, uint8_t u8)
{
  uint32_t x = (uint32_t)-1;                // UndCC_Valid - a cast of -1, not a subtraction
  uint8_t y = (uint8_t)+u8;                 // UndCC_Valid
  uint32_t w = (uint32_t)-1 + x;            // UndCC_Valid
  int32_t z = (a) - 1;                      // UndCC_Valid - parentheses around an operand
  int32_t v = (a) - x;                      // UndCC_Violation
  x = sizeof(int32_t) * a;                  // UndCC_Violation - sizeof gives unsigned, not a cast
  x = sizeof(int32_t) * 2U;                 // UndCC_Valid
  (void)y; (void)w; (void)z; (void)v;
}
