/* Test for MISRA C 2012 / 2023 / 2025 Rule 18.1 - A pointer resulting from
 * arithmetic on a pointer operand shall address an element of the same
 * array as that pointer operand.
 *
 * Conservative subset: detects statically decidable cases where the array
 * size is known at compile time and the index/offset is a compile-time
 * constant. Forming a one-past-the-end pointer (&arr[size], arr + size) is
 * permitted, but dereferencing it (arr[size], *(arr + size)) is flagged.
 * Library functions known to perform internal pointer arithmetic on their
 * parameters are also checked via OutOfBoundsStringFunctions.
 */

#include <stdint.h>
#include <string.h>

/* ===== Array subscript ===== */
void test_subscript(void)
{
  int arr[10];
  int x;

  x = arr[0];                /* UndCC_Valid */
  x = arr[9];                /* UndCC_Valid */
  x = arr[10];               /* UndCC_Violation dereference of one-past-end */
  x = arr[-1];               /* UndCC_Violation before start */
  x = arr[11];               /* UndCC_Violation two-past-end */
  (void)x;
}

/* ===== Multidimensional array subscripts ===== */
void test_multidimensional(void)
{
  int a2[5][3];
  int x;

  x = a2[0][0];              /* UndCC_Valid */
  x = a2[4][2];              /* UndCC_Valid */
  x = a2[4][3];              /* UndCC_Violation dereference one-past-end on inner dimension */
  x = a2[5][0];              /* UndCC_Violation dereference one-past-end on outer dimension */
  x = a2[4][4];              /* UndCC_Violation two-past-end on inner dimension */
  x = a2[6][0];              /* UndCC_Violation two-past-end on outer dimension */
  x = a2[-1][0];             /* UndCC_Violation before start on outer dimension */
  (void)x;
}

/* ===== Pointer arithmetic on array names: arr + n / arr - n ===== */
void test_ptr_arithmetic(void)
{
  int arr[10];

  (void)(arr + 0);           /* UndCC_Valid */
  (void)(arr + 10);          /* UndCC_Valid one-past-end */
  (void)(0 + arr);           /* UndCC_Valid commutative form */
  (void)(arr - 0);           /* UndCC_Valid */
  (void)(arr - (-10));       /* UndCC_Valid equivalent to arr + 10 (one-past-end) */

  (void)(arr + 11);          /* UndCC_Violation two-past-end */
  (void)(arr + (-1));        /* UndCC_Violation negative offset */
  (void)(11 + arr);          /* UndCC_Violation commutative two-past-end */
  (void)(arr - 1);           /* UndCC_Violation before start */
  (void)(arr - (-11));       /* UndCC_Violation equivalent to arr + 11 */
}

/* ===== &scalar arithmetic - scalar treated as array of size 1 ===== */
void test_addr_of_scalar(void)
{
  int i = 42;

  (void)(&i + 0);            /* UndCC_Valid */
  (void)(&i + 1);            /* UndCC_Valid one-past-end for size-1 "array" */

  (void)(&i + 2);            /* UndCC_Violation two-past-end */
  (void)(&i - 1);            /* UndCC_Violation before start */
}

/* ===== mem* library functions - caught by OutOfBoundsStringFunctions ===== */
void test_mem_functions(void)
{
  uint8_t buf1[16];
  uint8_t buf2[16];

  (void)memcmp(buf1, buf2, 16);   /* UndCC_Valid */
  (void)memcmp(buf1, buf2, 1);    /* UndCC_Valid */
  memcpy(buf1, buf2, 16);         /* UndCC_Valid */
  memmove(buf1, buf2, 8);         /* UndCC_Valid */

  (void)memcmp(buf1, buf2, 17);   /* UndCC_Violation size > buffer element count */
  memcpy(buf1, buf2, 32);         /* UndCC_Violation size > buffer element count */
  memmove(buf1, buf2, 17);        /* UndCC_Violation size > buffer element count */
}

/* ===== Struct member array - ConstantArrayType through member access ===== */
struct S { int data[10]; };

void test_struct_member(void)
{
  struct S s;
  int x;

  x = s.data[9];             /* UndCC_Valid */
  x = s.data[10];            /* UndCC_Violation dereference of one-past-end */
  x = s.data[11];            /* UndCC_Violation two-past-end */
  (void)x;
}

/* ===== String literal subscript - const char[N] is a ConstantArrayType ===== */
void test_string_literal(void)
{
  /* "hello" is const char[6] (5 chars + null terminator) */
  char c;

  c = "hello"[5];            /* UndCC_Valid last element (null terminator) */
  c = "hello"[6];            /* UndCC_Violation dereference of one-past-end */
  c = "hello"[7];            /* UndCC_Violation two-past-end */
  (void)c;
}

/* ===== Explicit dereference form - *(arr + n) hits PtrAdd, not Subscript ===== */
void test_explicit_dereference(void)
{
  int arr[10];
  int x;

  x = *(arr + 9);            /* UndCC_Valid */
  x = *(arr + 10);           /* UndCC_Violation dereference of one-past-end */
  x = *(arr + 11);           /* UndCC_Violation two-past-end */
  (void)(arr + 10);          /* UndCC_Valid one-past-end formation, not dereferenced */
  (void)x;
}

/* ===== Macro-expanded constant as index ===== */
#define VALID_IDX  9
#define BAD_IDX   15

void test_macro_index(void)
{
  int arr[10];
  int x;

  x = arr[VALID_IDX];        /* UndCC_Valid */
  x = arr[BAD_IDX];          /* UndCC_Violation index 15 > size 10 */
  (void)x;
}

/* ===== Address-of array element: &arr[N] - inner subscript is matched ===== */
void test_address_of_element(void)
{
  int arr[10];

  (void)&arr[0];             /* UndCC_Valid */
  (void)&arr[9];             /* UndCC_Valid */
  (void)&arr[10];            /* UndCC_Valid one-past-end via inner subscript */
  (void)&arr[11];            /* UndCC_Violation inner subscript 11 > size 10 */
  (void)&arr[-1];            /* UndCC_Violation inner subscript before start */
}

/* ===== Known false negatives - statically undecidable without flow analysis ===== */
void test_known_false_negatives(void)
{
  int arr[10];

  /* Chained arithmetic: checker sees p as an opaque pointer variable */
  int *p = arr + 5;
  (void)(p + 6);             /* UndCC_FalseNeg equivalent to arr + 11 */

  /* &arr[k] + n with a constant k: the checker knows &arr[0] addresses element
   * 0 of an array of size 10, so it can validate the offset. */
  (void)(&arr[0] + 11);     /* UndCC_Violation offset 11 past start of array */
}

/* ===== Known false negatives - operator forms the checker does not match =====
 * The Amplification lists ++p, p++, --p, p--, p+=n, p-=n as applicable forms,
 * but the checker only matches binaryOperator(+/-) and arraySubscriptExpr.
 * UnaryOperator and CompoundAssignOperator forms on pointers are not matched.
 */
void test_unmatched_operator_forms(void)
{
  int arr[10];
  int *p;

  p = arr + 11;              /* UndCC_Violation matched form */

  /* Unary increment / decrement on a pointer variable - not matched */
  p = arr; ++p;              /* UndCC_Valid - in range anyway */
  p = arr + 10; ++p;         /* UndCC_FalseNeg - now two-past-end */
  p = arr + 10; p++;         /* UndCC_FalseNeg - now two-past-end */
  p = arr; --p;              /* UndCC_FalseNeg - before start */
  p = arr; p--;              /* UndCC_FalseNeg - before start */

  /* Compound assignment on a pointer variable - not matched */
  p = arr; p += 11;          /* UndCC_FalseNeg - two-past-end */
  p = arr; p -= 1;           /* UndCC_FalseNeg - before start */
}

/* ===== Pointer parameters - checker cannot determine array size ===== */
void test_pointer_params(int *p, int *q)
{
  (void)(p + 100);           /* UndCC_Valid size unknown */
  (void)(p - 5);             /* UndCC_Valid size unknown */
  (void)(p[50]);             /* UndCC_Valid size unknown */
  (void)memcmp(p, q, 999);   /* UndCC_Valid size unknown */
}

/* ===== Direct ports of the MISRA C standard's own example functions =====
 * These mirror the f1, f2, f3 examples from the Rule 18.1 description so that
 * the test exercises the exact code the standard discusses. Cases that the
 * checker cannot decide statically are marked UndCC_Valid (when the size is
 * genuinely unknown, e.g. parameter arrays decay to pointers) or
 * UndCC_FalseNeg (when flow analysis would be needed).
 */

typedef int int32_t_local;
typedef unsigned short uint16_t_local;

/* f1: parameter int32_t a1 is a pointer, parameter int32_t a2[10] decays to a
 * pointer in C - so the checker cannot know the array size for either. */
int f1(int * const a1, int a2[10])
{
  int *p_local = &a1[3];     /* UndCC_Valid size of *a1 unknown */
  (void)p_local;
  return *(a2 + 9);           /* UndCC_Valid a2 decayed to pointer, size unknown */
}

void f2_from_standard(void)
{
  int data = 0;
  int b   = 0;
  int c[10] = { 0 };
  int d[5][2] = { {0} };

  int *p1 = &c[0];           /* UndCC_Valid */
  int *p2 = &c[10];          /* UndCC_Valid one-past-end pointer formation */
  int *p3 = &c[11];          /* UndCC_Violation inner subscript 11 > 10 */
  (void)p1; (void)p2; (void)p3;

  data = *p2;                 /* UndCC_FalseNeg dereference of one-past-end (flow analysis needed) */

  data = f1(&b, c);          /* UndCC_Valid */
  data = f1(c, c);           /* UndCC_Valid */

  c[-1] = 0;                  /* UndCC_Violation array bounds exceeded */
  data = c[10];               /* UndCC_Violation dereference of address one beyond (matches the standard's own example) */
  data = *(&data + 0);        /* UndCC_Valid data treated as array of size 1 */

  d[3][1] = 0;                /* UndCC_Valid */
  data = *(*(d + 3) + 1);    /* UndCC_Valid */
  data = d[2][3];             /* UndCC_Violation internal boundary exceeded (3 > inner size 2) */

  p1 = d[1];                 /* UndCC_Valid */
  data = p1[1];               /* UndCC_Valid p1 is a pointer variable, size unknown */
  (void)data;
}

/* f3: struct member arithmetic. The standard's intent is to forbid arithmetic
 * that crosses from one struct member to another. The checker cannot track
 * this because p is a pointer variable - all such cases are false negatives. */
struct standard_struct {
  unsigned short x;
  unsigned short y;
  unsigned short z;
  unsigned short a[10];
};

void f3_from_standard(void)
{
  struct standard_struct s;
  unsigned short *p_local;

  p_local = &s.x;
  p_local[0] = 1;             /* UndCC_FalseNeg dereference of one-beyond s.x (flow analysis needed) */
  p_local[1] = 2;             /* UndCC_FalseNeg crosses into s.y (flow analysis needed) */

  p_local = &s.a[0];
  p_local = p_local + 8;     /* UndCC_Valid checker sees p_local as opaque, no flag */
  p_local = p_local + 3;     /* UndCC_FalseNeg crosses end of s.a (flow analysis needed) */
  (void)p_local;
}
