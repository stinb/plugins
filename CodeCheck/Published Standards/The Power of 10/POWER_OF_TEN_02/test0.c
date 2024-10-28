// Mostly for loops

#include <stdbool.h>
#include <stdlib.h>

enum { LIMIT = 8 };

void for_fixed_upper_bound()
{
  int limit = 8;
  for (int i = 0; i < limit; i++) { // UndCC_Valid - fixed upper limit

  }

  for (int i = 7; i > -1; i--) { // UndCC_Valid - fixed upper limit

  }

  #define limit 8
  for (int i = 0; i < limit; i++) { // UndCC_Valid - fixed upper limit

  }
  #undef limit

  int i;
  for (i = 0; i < LIMIT; i++) { // UndCC_Valid - fixed upper limit

  }
}

void for_bad_logic_upper_bound()
{
  int x = 3;
  for (int i = 0; i < 8, x > 2; i++) { // UndCC_Violation - i < 8 not used

  }

  for (int i = 0; i < 8; i) { // UndCC_Violation - i not iterated

  }

  for (unsigned int i = 10; i >= 0; i--) { // UndCC_FalseNeg - covered by Rule 10

  }
}

void for_modified_upper_bound()
{
  int limit = 8;
  for (int i = 0; i < limit; i++) { // UndCC_Violation - limit is modified
    if (i == 4)
      limit <<= 1;
  }
}

void for_volatile_upper_bound()
{
  int volatile limit = 8;
  for (int i = 0; i < limit; i++) { // UndCC_Violation - limit is volatile

  }

  volatile int i;
  for (i = 0; i < 8; i++) { // UndCC_Violation - i is volatile

  }
}

void for_no_upper_bound()
{
  int limit = 8;
  for (int i = 0; limit; i++) { // UndCC_Violation - limit not used

  }
}

void unconditional_infinite()
{
  for (;;) { // UndCC_Valid - allowed for the simple case (1)

  }

  while (1) { // UndCC_Valid - allowed for the simple case (1)

  }

  do { // UndCC_Valid - allowed for the simple case (1)

  } while (true);
}

int conditional_infinite(int condition)
{
  for (;;) { // UndCC_Violation - "infinite" loop has break
    if (condition)
      break;
  }

  while (1) { // UndCC_Violation - "infinite" loop has return
    if (condition)
      return 0;
  }

  do { // UndCC_Violation - "infinite" loop has goto
    if (condition)
      goto label;
  } while (1);

  while (1) { // UndCC_Violation - "infinite" loop has exit
    if (condition)
      exit(0);
  }

label:
  return 0;
}
