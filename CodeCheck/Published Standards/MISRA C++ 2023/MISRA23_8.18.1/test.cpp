typedef short int16_t;
typedef int int32_t;
typedef unsigned long size_t;

namespace std {
    template<typename T, size_t N>
    struct array {
        T data[N];
        T& operator[](size_t i) { return data[i]; }
    };
}

void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);

void f1(void)
{
    union
    {
        int16_t i;
        int32_t j;
    } a = {0};
    a.i = a.i; // Rule does not apply - same member
    a.j = a.i; // UndCC_Violation
}
void f2(std::array<int16_t, 20> &a)
{
    memcpy(&a[0], &a[1], 10u * sizeof(a[0]));  // UndCC_Violation(memcpy)
    memmove(&a[0], &a[1], 10u * sizeof(a[0])); // Rule does not apply
    memcpy(&a[1], &a[0], 10u * sizeof(a[0]));  // UndCC_Violation(memcpy)
    memmove(&a[1], &a[0], 10u * sizeof(a[0])); // Rule does not apply
    memcpy(&a[0], &a[5], 5u * sizeof(a[0]));   // UndCC_FalsePos(memcpy) - no overlap
}
