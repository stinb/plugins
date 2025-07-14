#include <stdint.h>
#include <stdbool.h>

extern void f(signed int a);                   // UndCC_Violation(c, d)
void f(int a);                                 // UndCC_Violation(c, d)
extern void g(signed int b);                   /* UndCC_Violation(b, d) - Non-compliant if unnamed option is set to False*/
extern void g(signed int);                     /* UndCC_Violation(b, d) - Non-compliant if unnamed option is set to False*/
extern void h(int *const c);                   /* UndCC_Violation */
extern void h(int *c);                         /* UndCC_Violation - mis-matched type qualifiers */
extern void j(int d);                          /* UndCC_Violation */
extern void j(int e);                          /* UndCC_Violation - mis-matched parameter names */
extern int16_t func(int16_t num, int16_t den); /* UndCC_Violation */

int16_t func(int16_t den, int16_t num) /* UndCC_Violation - parameter names do not match */
{
    return num / den;
}

typedef uint16_t width_t;
typedef uint16_t height_t;
typedef uint32_t area_t;
extern area_t area(width_t w, height_t h); /* UndCC_Violation */
area_t area(width_t w, width_t h)          /* UndCC_Violation */
{
    return (area_t)w * h;
}

extern void f1(int16_t x);
extern void f2(int16_t y);
void f3(bool b)
{
    void (*fp1)(int16_t z) = b ? f1 : f2;
}
