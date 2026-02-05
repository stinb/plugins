#include <cstdint>
#include <cstring>
#include <iterator>

// ---------------------------------------------------------------------------
// Array subscript
// ---------------------------------------------------------------------------
void test_subscript()
{
  int arr[10];
  int x;

  x = arr[0];                // UndCC_Valid
  x = arr[9];                // UndCC_Valid
  x = arr[10];               // UndCC_Valid one-past-end
  x = arr[-1];               // UndCC_Violation before start
  x = arr[11];               // UndCC_Violation two-past-end
  (void)x;
}

// ---------------------------------------------------------------------------
// Multidimensional array subscripts
// ---------------------------------------------------------------------------
void test_multidimensional()
{
  int a2[5][3];
  int x;

  x = a2[0][0];              // UndCC_Valid
  x = a2[4][2];              // UndCC_Valid
  x = a2[4][3];              // UndCC_Valid one-past-end on inner dimension
  x = a2[5][0];              // UndCC_Valid one-past-end on outer dimension
  x = a2[4][4];              // UndCC_Violation two-past-end on inner dimension
  x = a2[6][0];              // UndCC_Violation two-past-end on outer dimension
  x = a2[-1][0];             // UndCC_Violation before start on outer dimension
  (void)x;
}

// ---------------------------------------------------------------------------
// Pointer arithmetic on array names: arr + n / arr - n
// ---------------------------------------------------------------------------
void test_ptr_arithmetic()
{
  int arr[10];

  (void)(arr + 0);           // UndCC_Valid
  (void)(arr + 10);          // UndCC_Valid one-past-end
  (void)(0 + arr);           // UndCC_Valid commutative form
  (void)(arr - 0);           // UndCC_Valid
  (void)(arr - (-10));       // UndCC_Valid equivalent to arr + 10 (one-past-end)

  (void)(arr + 11);          // UndCC_Violation two-past-end
  (void)(arr + (-1));        // UndCC_Violation negative offset
  (void)(11 + arr);          // UndCC_Violation commutative two-past-end
  (void)(arr - 1);           // UndCC_Violation before start
  (void)(arr - (-11));       // UndCC_Violation equivalent to arr + 11
}

// ---------------------------------------------------------------------------
// &scalar arithmetic — scalar treated as array of size 1
// ---------------------------------------------------------------------------
void test_addr_of_scalar()
{
  int i = 42;

  (void)(&i + 0);            // UndCC_Valid
  (void)(&i + 1);            // UndCC_Valid one-past-end for size-1 "array"

  (void)(&i + 2);            // UndCC_Violation two-past-end
  (void)(&i - 1);            // UndCC_Violation before start
}

// ---------------------------------------------------------------------------
// std::next on array names
// ---------------------------------------------------------------------------
void test_std_next()
{
  int arr[10];

  (void)std::next(arr, 0);   // UndCC_Valid
  (void)std::next(arr, 5);   // UndCC_Valid
  (void)std::next(arr, 10);  // UndCC_Valid one-past-end

  (void)std::next(arr, 11);  // UndCC_Violation two-past-end
  (void)std::next(arr, -1);  // UndCC_Violation before start
}

// ---------------------------------------------------------------------------
// mem* library functions — caught by OutOfBoundsStringFunctions via keys.
// Use uint8_t buffers so element count == byte count.
// Do NOT test size == 0 — that is an OutOfBoundsStringFunctions warning but
// not an 8.7.1 violation.
// ---------------------------------------------------------------------------
void test_mem_functions()
{
  uint8_t buf1[16];
  uint8_t buf2[16];

  (void)memcmp(buf1, buf2, 16);   // UndCC_Valid
  (void)memcmp(buf1, buf2, 1);    // UndCC_Valid
  memcpy(buf1, buf2, 16);         // UndCC_Valid
  memmove(buf1, buf2, 8);         // UndCC_Valid

  (void)memcmp(buf1, buf2, 17);   // UndCC_Violation size > buffer element count
  memcpy(buf1, buf2, 32);         // UndCC_Violation size > buffer element count
  memmove(buf1, buf2, 17);        // UndCC_Violation size > buffer element count
}

// ---------------------------------------------------------------------------
// Struct member array — ConstantArrayType through member access
// ---------------------------------------------------------------------------
struct S { int data[10]; };

void test_struct_member()
{
  S s;
  int x;

  x = s.data[9];             // UndCC_Valid
  x = s.data[10];            // UndCC_Valid one-past-end
  x = s.data[11];            // UndCC_Violation two-past-end
  (void)x;
}

// ---------------------------------------------------------------------------
// String literal subscript — const char[N] is a ConstantArrayType
// ---------------------------------------------------------------------------
void test_string_literal()
{
  // "hello" is const char[6] (5 chars + null terminator)
  char c;

  c = "hello"[5];            // UndCC_Valid last element (null terminator)
  c = "hello"[6];            // UndCC_Valid one-past-end
  c = "hello"[7];            // UndCC_Violation two-past-end
  (void)c;
}

// ---------------------------------------------------------------------------
// Explicit dereference form — *(arr + n) hits PtrAdd, not Subscript
// ---------------------------------------------------------------------------
void test_explicit_dereference()
{
  int arr[10];
  int x;

  x = *(arr + 9);            // UndCC_Valid
  x = *(arr + 11);           // UndCC_Violation two-past-end
  (void)x;
}

// ---------------------------------------------------------------------------
// Macro-expanded constant as index — isMacroID is on the subscript begin
// loc (the array name), not the index, so the macro offset is still evaluated
// ---------------------------------------------------------------------------
#define VALID_IDX  9
#define BAD_IDX   15

void test_macro_index()
{
  int arr[10];
  int x;

  x = arr[VALID_IDX];        // UndCC_Valid
  x = arr[BAD_IDX];          // UndCC_Violation index 15 > size 10
  (void)x;
}

// ---------------------------------------------------------------------------
// Known false negatives — statically undecidable without flow analysis
// ---------------------------------------------------------------------------
void test_known_false_negatives()
{
  int arr[10];

  // Chained arithmetic: checker sees p as an opaque pointer variable
  int *p = arr + 5;
  (void)(p + 6);             // UndCC_FalseNeg equivalent to arr + 11

  // &arr[0] + n: getKnownArraySize deliberately returns -1 for &arr[subscript]
  // to avoid false positives when the subscript is non-zero
  (void)(&arr[0] + 11);     // UndCC_FalseNeg offset 11 past start of array
}

// ---------------------------------------------------------------------------
// Pointer parameters — checker cannot determine array size; no violations
// ---------------------------------------------------------------------------
void test_pointer_params(int *p, int *q)
{
  (void)(p + 100);           // UndCC_Valid size unknown
  (void)(p - 5);             // UndCC_Valid size unknown
  (void)(p[50]);             // UndCC_Valid size unknown
  (void)std::next(p, 200);  // UndCC_Valid size unknown
  (void)memcmp(p, q, 999);  // UndCC_Valid size unknown
}
