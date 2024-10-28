#include <ctype.h>
#include <limits.h>
#include <stdint.h>

// Test all of the functions listed in <ctype.h>

void bad()
{
  uint32_t ch = (uint32_t) -1ul;

  (void) isalnum(ch);  // UndCC_Violation(Win, Lin)
  (void) isalpha(ch);  // UndCC_Violation(Win, Lin)
  (void) islower(ch);  // UndCC_Violation(Win, Lin)
  (void) isupper(ch);  // UndCC_Violation(Win, Lin)
  (void) isdigit(ch);  // UndCC_Violation(Win, Lin)
  (void) isxdigit(ch); // UndCC_Violation(Win, Lin)
  (void) iscntrl(ch);  // UndCC_Violation(Win, Lin)
  (void) isgraph(ch);  // UndCC_Violation(Win, Lin)
  (void) isspace(ch);  // UndCC_Violation(Win, Lin)
  (void) isblank(ch);  // UndCC_Violation(Win, Lin)
  (void) isprint(ch);  // UndCC_Violation(Win, Lin)
  (void) ispunct(ch);  // UndCC_Violation(Win, Lin)
  (void) tolower(ch);  // UndCC_Violation(Win, Lin)
  (void) toupper(ch);  // UndCC_Violation(Win, Lin)
}

void good()
{
  uint32_t ch = (uint32_t) -1ul;

  if (ch > SCHAR_MAX) {
    // Handle error
  } else {
    (void) isalnum(ch);
    (void) isalpha(ch);
    (void) islower(ch);
    (void) isupper(ch);
    (void) isdigit(ch);
    (void) isxdigit(ch);
    (void) iscntrl(ch);
    (void) isgraph(ch);
    (void) isspace(ch);
    (void) isblank(ch);
    (void) isprint(ch);
    (void) ispunct(ch);
    (void) tolower(ch);
    (void) toupper(ch);
  }
}
