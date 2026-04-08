#include <stdint.h>


// Non-compliant: read different member than last written (type punning)
uint32_t floatrep( float flt )
{
  union
  {
    float    flt;
    uint32_t bits;
  } tmp;

  tmp.flt = flt;
  return tmp.bits;     // UndCC_Violation - read of different member
}


// Non-compliant: x was not the most recently written member
uint32_t zerobits( uint32_t x )
{
  union
  {
    uint32_t x;
    uint16_t lo;
  } tmp;

  tmp.x  = x;
  tmp.lo = 0;
  return tmp.x;        // UndCC_Violation - x not most recently set
}


// Compliant by exception: character array member
uint8_t floatbytes( float flt )
{
  union
  {
    float   flt;
    uint8_t bytes[ sizeof(float) ];
  } tmp;

  tmp.flt = flt;
  return tmp.bytes[1]; // UndCC_Valid - character array exception
}


// Compliant: same member read as written
void compliant_same_member( void )
{
  union
  {
    int   i;
    float f;
  } u;

  u.i = 42;
  int x = u.i;         // UndCC_Valid
  (void)x;
}


// Non-compliant: member never set before read
void unset_member( void )
{
  union
  {
    int   i;
    float f;
  } u;

  int x = u.i;         // UndCC_Violation - never set
  (void)x;
}


// Compliant: named union type
union Data
{
  int32_t  ival;
  float    fval;
};

void named_union_compliant( void )
{
  union Data d;
  d.ival = 10;
  int32_t x = d.ival;  // UndCC_Valid
  (void)x;
}


// Non-compliant: named union type, different member
void named_union_noncompliant( void )
{
  union Data d;
  d.ival = 10;
  float x = d.fval;    // UndCC_Violation
  (void)x;
}


// Compliant: multiple writes, read the last one written
void multiple_writes_compliant( void )
{
  union
  {
    int   i;
    float f;
  } u;

  u.i = 1;
  u.f = 2.0f;
  float x = u.f;       // UndCC_Valid - f was most recently set
  (void)x;
}


// Compliant: re-set original member after different member was set
void reset_member_compliant( void )
{
  union
  {
    int   i;
    float f;
  } u;

  u.i = 1;
  u.f = 2.0f;
  u.i = 3;
  int x = u.i;         // UndCC_Valid - i was re-set most recently
  (void)x;
}


// Typedef'd union
typedef union
{
  int32_t  ival;
  float    fval;
} DataTypedef;

void typedef_union_noncompliant( void )
{
  DataTypedef d;
  d.ival = 10;
  float x = d.fval;    // UndCC_Violation
  (void)x;
}

void typedef_union_compliant( void )
{
  DataTypedef d;
  d.fval = 1.0f;
  float x = d.fval;    // UndCC_Valid
  (void)x;
}


// Character array exception: char[] member
void char_array_exception( void )
{
  union
  {
    int32_t val;
    char    bytes[4];
  } u;

  u.val = 0x12345678;
  char c = u.bytes[0]; // UndCC_Valid - char array exception
  (void)c;
}


// Character array exception: unsigned char[] member
void unsigned_char_array_exception( void )
{
  union
  {
    int32_t       val;
    unsigned char bytes[4];
  } u;

  u.val = 0x12345678;
  unsigned char c = u.bytes[0]; // UndCC_Valid - unsigned char array exception
  (void)c;
}


// Character array exception: signed char[] member
void signed_char_array_exception( void )
{
  union
  {
    int32_t     val;
    signed char bytes[4];
  } u;

  u.val = 0x12345678;
  signed char c = u.bytes[0]; // UndCC_Valid - signed char array exception
  (void)c;
}


// Designated initializer: compliant (read same member that was initialized)
void designated_init_compliant( void )
{
  union
  {
    int   i;
    float f;
  } u = { .i = 42 };

  int x = u.i;         // UndCC_Valid - i was initialized
  (void)x;
}


// Designated initializer: non-compliant (read different member)
void designated_init_noncompliant( void )
{
  union
  {
    int   i;
    float f;
  } u = { .i = 42 };

  float x = u.f;       // UndCC_Violation - i was set, reading f
  (void)x;
}


// Struct within union: compliant (same top-level member)
void struct_in_union_compliant( void )
{
  union
  {
    struct { int a; int b; } s;
    int i;
  } u;

  u.s.a = 1;
  u.s.b = 2;
  int x = u.s.a;       // UndCC_Valid - s was most recently accessed
  (void)x;
}


// Struct within union: non-compliant (different top-level member)
void struct_in_union_noncompliant( void )
{
  union
  {
    struct { int a; int b; } s;
    int i;
  } u;

  u.s.a = 1;
  int x = u.i;         // UndCC_Violation - s was set, reading i
  (void)x;
}


// sizeof does not read the value at runtime
void sizeof_not_a_read( void )
{
  union
  {
    float   f;
    double  d;
    char    buff[sizeof(double)];
  } u;

  if (sizeof(u.f) == 4) { ; } // UndCC_Valid - sizeof is not a read
  u.f = 1.0f;
  if (sizeof(u.d) == 8) { ; } // UndCC_Valid - sizeof is not a read
  float x = u.f;              // UndCC_Valid - f was most recently set
  (void)x;
}


// Whole-object assignment sets all members
void whole_object_copy( void )
{
  union
  {
    int   i;
    float f;
  } u, v;

  v.i = 10;
  u = v;
  int x = u.i;           // UndCC_Valid - whole-object assignment
  (void)x;
}


// Exception is read-only: writing char array then reading non-char is still a violation
void char_array_write_nonchar_read( void )
{
  union
  {
    int32_t val;
    char    bytes[4];
  } u;

  u.bytes[0] = 0;
  int32_t x = u.val;     // UndCC_Violation - char array exception only applies to reads
  (void)x;
}


// Recursive rule: struct sub-members should be tracked individually
void struct_submember_not_set( void )
{
  union
  {
    struct { int a; int b; } s;
    int i;
  } u;

  u.s.a = 1;
  int x = u.s.b;         // UndCC_FalseNeg - b was never set, only a was
  (void)x;
}
