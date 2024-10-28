// Custom

#include <cstdint>

typedef char char_t;

typedef bool boolean;
typedef boolean boolean_indirect;

typedef uint32_t uint32_indirect;

typedef signed int i32_typedef_t;
using i32_using_t = signed int;

struct SomeStruct {
  int invalidField : 4; // UndCC_Violation
  signed int validSigned : 2;
  unsigned int validUnsigned : 1;
  char invalidChar : 2; // UndCC_Violation
  unsigned char invalidUnsignedChar : 2;
  signed char invalidSignedChar : 2;
  bool validBool : 1;
  boolean validBoolean : 1;
  boolean_indirect validIndirectBoolean : 1;
  uint32_t validUint32 : 8;
  char_t invalidCharT : 3; // UndCC_Violation
  uint32_indirect validIndirectUint32 : 7;
  i32_typedef_t validI32Typdef : 9;
  i32_using_t validI32Using : 10;
};

struct NonBitFieldObject {
  int validField = 4;
};

struct CommaAssignment {
  signed a : 1,
         b : 1;
};

struct Unnamed
{
  unsigned : 1;
  unsigned int : 1;
  signed : 1;
  signed int : 1;
  int : 1; // UndCC_Violation - still accessible through pointer witchcraft
};

typedef unsigned int bitfield_t;

typedef struct {
  bitfield_t   valid1 : 1;
  bitfield_t   valid2 : 1;
  unsigned int valid3 : 1;
} test_t;

test_t test_var;
