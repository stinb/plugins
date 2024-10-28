#include <stdint.h>
#include <stdbool.h>

extern void f(signed int a); /* UndCC_FalseNeg */
void f(int a); /* UndCC_FalseNeg - Exception 1 */
extern void g(signed int b); /* UndCC_Violation(a) */
extern void g(signed int); /* UndCC_Violation(a) - Exception 2 */
extern void h(int *const c); /* UndCC_Violation(a) */
extern void h(int *c); /* UndCC_Violation(a) - mis-matched type qualifiers */
extern void j(int d); /* UndCC_Violation(a) */
extern void j(int e); /* UndCC_Violation(a) - mis-matched parameter names */
extern int16_t func(int16_t num, int16_t den); /* UndCC_Violation(a) */

int16_t func(int16_t den, int16_t num) /* UndCC_Violation(a) - parameter names do not match */
{
    return num / den;
}

typedef uint16_t width_t;
typedef uint16_t height_t;
typedef uint32_t area_t;
extern area_t area(width_t w, height_t h); /* UndCC_Violation(a) */
area_t area(width_t w, width_t h) /* UndCC_Violation(a) */
{
    return (area_t)w * h;
}

extern void f1(int16_t x);
extern void f2(int16_t y);
void f3(bool b)
{
    void (*fp1)(int16_t z) = b ? f1 : f2;
}
