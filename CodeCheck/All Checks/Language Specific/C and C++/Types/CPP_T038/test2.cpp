// Typedefs

#include <cstdint>

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef int32_t  int32;
typedef uint32   uint32_alias; // two typedefs away from a fixed width integer

typedef unsigned short my_word;  // allowed by pattern
typedef unsigned short raw_word; // not allowed

struct S
{
  uint16 m1;
  uint32 m2;
};

uint32 g;

void test2_a()
{
  uint8 a;
  uint16 b;
  uint32 c;
  int32 d;
  uint32_alias e;
  uint8 arr[4];
  const uint16 f = 0;
  uint32 &h = c;
  int bad; // UndCC_Violation
  int32_t ok;
}

void test2_b()
{
  my_word a;  // UndCC_Violation(allow_char,allow_size_t,strict)
  raw_word b; // UndCC_Violation
}

// The "char" pattern matches part of the underlying type "unsigned char", so
// it must not keep that type from being removed whole
void test2_c()
{
  uint8 a;   // UndCC_Valid
  uint8_t b; // UndCC_Valid
  int8_t c;  // UndCC_Valid
}
