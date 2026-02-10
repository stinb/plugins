// Custom

#include <stdint.h>
#include <memory>

struct Sized
{
  uint16_t len;
  uint32_t data[10];              // UndCC_Valid
};

struct SizedIndirect
{
  using SizedU32 = uint32_t[10];
  uint16_t len;
  SizedU32 data;                  // UndCC_Valid
};

struct FlexibleIndirect
{
  using FlexibleU32 = uint32_t[];
  uint16_t len;
  FlexibleU32 data;               // UndCC_Violation
};

union FlexibleUnion
{
  uint16_t len;
  uint32_t data[];                // UndCC_Violation
};

class FlexibleClass
{
  uint16_t len;
  uint32_t data[];                // UndCC_Violation
};

struct SizedStatic
{
  int len;
  static const int data[];        // UndCC_Valid
};

const int SizedStatic::data[] = { // UndCC_Valid
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
};

class NewArray
{
  size_t size;
  std::unique_ptr<char []> data;  // UndCC_Valid
};
