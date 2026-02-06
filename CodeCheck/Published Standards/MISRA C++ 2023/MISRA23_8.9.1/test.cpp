#include <functional>
#include <cstdint>

// Test 1: Basic array comparisons (from standard example)
void f1()
{
    int32_t a1[10];
    int32_t a2[10];
    int32_t *p1 = a1;

    if (p1 < a1) {} // UndCC_Valid - same array
    if (p1 < &a1[9]) {} // UndCC_Valid - same array
    if (p1 < a2) {} // UndCC_Valid - undecidable (requires data-flow to track p1 initialization)
    if (p1 > a2) {} // UndCC_Valid - undecidable (requires data-flow to track p1 initialization)
    if (p1 <= a2) {} // UndCC_Valid - undecidable (requires data-flow to track p1 initialization)
    if (p1 >= a2) {} // UndCC_Valid - undecidable (requires data-flow to track p1 initialization)
}

// Test 2: std::less and related functors are compliant (from standard)
void f1b()
{
    int32_t a1[10];
    int32_t a2[10];
    int32_t *p1 = a1;

    if (std::less<>{}(p1, a2)) {} // UndCC_Valid - std::less is allowed
    if (std::less_equal<>{}(p1, a2)) {} // UndCC_Valid - std::less_equal is allowed
    if (std::greater<>{}(p1, a2)) {} // UndCC_Valid - std::greater is allowed
    if (std::greater_equal<>{}(p1, a2)) {} // UndCC_Valid - std::greater_equal is allowed
}

// Test 3: Struct member comparisons (from standard example)
struct S {
    int32_t m1;
    int32_t m2;
};

void f2()
{
    S x{};
    if (&x.m1 <= &x.m2) {} // UndCC_Violation - members are not array elements
    if (&x.m1 < &x.m2) {} // UndCC_Violation - members are not array elements
    if (&x.m1 >= &x.m2) {} // UndCC_Violation - members are not array elements
}

// Test 4: Array with one-past-the-end pointer (compliant per note 1)
void f3()
{
    int32_t arr[10];
    int32_t *p = arr;
    int32_t *end = arr + 10; // One past the end

    if (p < end) {} // UndCC_Valid - one-past-the-end is part of array
    if (end > arr) {} // UndCC_Valid - one-past-the-end is part of array
}

// Test 5: Single object treated as array of one (note 2)
void f4()
{
    int32_t x;
    int32_t y;
    int32_t *px = &x;
    int32_t *py = &y;

    if (px < py) {} // UndCC_Valid - undecidable (requires data-flow to track px, py initialization)
    if (px < &x + 1) {} // UndCC_Valid - one-past-the-end of single object
}

// Test 6: Pointer arithmetic on same array
void f5()
{
    int32_t arr[20];
    int32_t *p1 = arr + 5;
    int32_t *p2 = arr + 10;

    if (p1 < p2) {} // UndCC_Valid - same array
    if (p1 <= p2) {} // UndCC_Valid - same array
}

// Test 7: Global arrays
int32_t g_arr1[10];
int32_t g_arr2[10];

void f6()
{
    if (g_arr1 < g_arr2) {} // UndCC_Violation - different arrays
    if (&g_arr1[5] < &g_arr1[8]) {} // UndCC_Valid - same array
}

// Test 8: Dynamically allocated arrays (conservative - may not detect)
void f7()
{
    int32_t *arr1 = new int32_t[10];
    int32_t *arr2 = new int32_t[10];

    // Undecidable - pointer variables; requires data-flow to track allocations
    if (arr1 < arr2) {} // UndCC_Valid - undecidable
    if (arr1 + 5 < arr2 + 3) {} // UndCC_Valid - undecidable

    delete[] arr1;
    delete[] arr2;
}

// Test 9: Function parameters (undecidable - should not flag)
void f8(int32_t *p1, int32_t *p2)
{
    // Cannot determine if p1 and p2 point to same array
    if (p1 < p2) {} // UndCC_Valid - undecidable, don't flag
}

// Test 10: Array of structs
void f9()
{
    S arr[10];
    S *p1 = &arr[0];
    S *p2 = &arr[5];

    if (p1 < p2) {} // UndCC_Valid - same array
}

// Test 11: Nested structs
struct Outer {
    int32_t field1;
    struct Inner {
        int32_t a;
        int32_t b;
    } inner;
    int32_t field2;
};

void f10()
{
    Outer obj;
    if (&obj.field1 < &obj.field2) {} // UndCC_Violation - different members
    if (&obj.inner.a < &obj.inner.b) {} // UndCC_Violation - different members of nested struct
}

// Test 12: Equality operators are NOT covered by this rule
void f11()
{
    int32_t a1[10];
    int32_t a2[10];
    int32_t *p1 = a1;

    if (p1 == a2) {} // UndCC_Valid - equality not covered by rule
    if (p1 != a2) {} // UndCC_Valid - inequality not covered by rule
}

// Test 13: Array members of struct
struct WithArrays {
    int32_t arr1[10];
    int32_t arr2[10];
};

void f12()
{
    WithArrays obj;
    if (obj.arr1 < obj.arr2) {} // UndCC_Violation - different array members
    if (&obj.arr1[5] < &obj.arr1[8]) {} // UndCC_Valid - same array member
    if (&obj.arr1[5] < &obj.arr2[8]) {} // UndCC_Violation - different array members
}

// Test 14: Multi-dimensional arrays
// Known limitation: checker cannot distinguish between sub-arrays (matrix[0] vs matrix[1])
// because getRootDecl() returns the base array for both
void f13()
{
    int32_t matrix[3][10];

    // Comparing sub-arrays - each row is a separate array
    if (matrix[0] < matrix[1]) {} // UndCC_FalseNeg - different sub-arrays (not detected)
    if (&matrix[0][5] < &matrix[0][8]) {} // UndCC_Valid - same sub-array
    if (&matrix[0][5] < &matrix[1][2]) {} // UndCC_FalseNeg - different sub-arrays (not detected)

    // Same sub-array with pointer arithmetic
    int32_t *row0 = matrix[0];
    int32_t *row1 = matrix[1];
    if (row0 < row0 + 5) {} // UndCC_Valid - same sub-array
}

// Test 15: String literals (character arrays)
void f14()
{
    const char* s1 = "MISRA";
    const char* s2 = "C++";

    // Comparisons within same string literal - compliant
    if (s1 < (s1 + 2)) {} // UndCC_Valid - same string literal
    if (s1 < (s1 + 5)) {} // UndCC_Valid - one past the end of same literal

    // Comparisons between different string literals - undecidable
    if (s1 < s2) {} // UndCC_Valid - undecidable (different literals, but pointer variables)

    // Direct literal comparison
    const char *lit1 = "Test";
    const char *lit2 = "Test";
    if (lit1 < lit2) {} // UndCC_Valid - undecidable (may be same literal due to string pooling)

    // Direct string literal comparison (unspecified behavior)
    if ("MISRA" < "C++") {} // UndCC_Violation - different string literals
    if ("Test" < "Test") {} // UndCC_Valid - same literal (same pointer)
}
