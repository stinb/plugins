// MISRA C++ 2008 - 5-0-13

#include <cstdint>

extern std::int32_t *fn();
extern std::int32_t fn2();
extern bool fn3();

void f(std::int32_t i, std::int32_t *p, double d, bool b, std::uint8_t u)
{
  while (std::int32_t *q = fn()) { (void)q; break; }            // UndCC_Valid - exception
  while (std::int32_t length = fn2()) { (void)length; break; }  // UndCC_Valid - exception
  while (bool flag = fn3()) { (void)flag; break; }              // UndCC_Valid

  // The cumbersome but compliant form from the standard
  do {
    std::int32_t *q = fn();
    if (nullptr == q) { break; }
  } while (true);            // UndCC_Valid

  if (i) { }                 // UndCC_Violation
  while (i) { break; }       // UndCC_Violation
  for (; i; ) { break; }     // UndCC_Violation
  do { } while (i);          // UndCC_Violation
  if (p) { }                 // UndCC_Violation - pointer condition
  if (d) { }                 // UndCC_Violation - floating condition

  if (i != 0) { }                // UndCC_Valid
  if (b) { }                     // UndCC_Valid
  if (static_cast<bool>(i)) { }  // UndCC_Valid
  if (!i) { }                    // UndCC_Valid - the condition has type bool
  if (u && (b <= b)) { }         // UndCC_Valid - the condition has type bool
  for (std::int32_t x = 0; x < 10; ++x) { }  // UndCC_Valid
  for (;;) { break; }            // UndCC_Valid - no condition
}
