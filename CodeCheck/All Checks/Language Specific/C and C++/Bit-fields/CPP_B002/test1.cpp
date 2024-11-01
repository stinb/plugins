// Custom

#include <cstdint>

typedef bool boolean;
typedef boolean boolean_indirect;
typedef char char_t;

typedef uint32_t uint32_indirect;

enum AnEnumType {
  A,
  B,
  C,
};

enum SignedEnum : int32_t {};
enum UnsignedEnum : uint32_t {};

typedef AnEnumType AnIndirectEnumType;

struct SomeStruct {
  int validField : 4;                         // UndCC_Valid
  signed int validSigned : 2;                 // UndCC_Valid
  unsigned int validUnsigned: 1;              // UndCC_Valid
  char validChar : 2;                         // UndCC_Valid
  unsigned char validUnsignedChar : 2;        // UndCC_Valid
  signed char validSignedChar : 2;            // UndCC_Valid
  bool validBool : 1;                         // UndCC_Valid
  boolean validBoolean : 1;                   // UndCC_Valid
  boolean_indirect validIndirectBoolean : 1;  // UndCC_Valid
  uint32_t validUint32 : 8;                   // UndCC_Valid
  char_t validCharT : 3;                      // UndCC_Valid
  uint32_indirect validIndirectUint32 : 7;    // UndCC_Valid
};

struct OtherStruct
{
   AnEnumType n; // UndCC_Valid
};
