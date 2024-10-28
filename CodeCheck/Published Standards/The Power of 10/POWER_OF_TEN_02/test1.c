// Mostly while loops and do-while loops

#include <stdbool.h>
#include <stdlib.h>

enum { LIMIT = 8 };

void while_fixed_upper_bound()
{
  int i = 0;
  int limit = 8;
  while (i < limit) { // UndCC_Valid - fixed upper limit
    i++;
  }

  i = 7;
  do { // UndCC_Valid - fixed upper limit
    i--;
  } while (i > -1);

  i = 0;
  #define limit 8
  while (i < limit) { // UndCC_Valid - fixed upper limit
    i++;
  }
  #undef limit

  i = 0;
  do { // UndCC_Valid - fixed upper limit
    i++;
  } while (i < LIMIT);
}

void while_bad_logic_upper_bound(int condition)
{
  int i = 0;
  int x = 3;
  while (i < 8, x > 2) { // UndCC_Violation - i < 8 not used
    i++;
  }

  i = 0;
  do { // UndCC_Violation - i may not be iterated
    if (condition)
      i++;
  } while (i < 8);

  unsigned int u = 10;
  while (u >= 0) { // UndCC_FalseNeg - covered by Rule 10
    u--;
  }
}

void while_modified_upper_bound()
{
  int i = 0;
  int limit = 8;
  while (i < limit) { // UndCC_Violation - limit is modified
    i++;
    if (i == 4)
      limit <<= 1;
  }
}

void while_volatile_upper_bound()
{
  int i = 0;
  int volatile limit = 8;
  do { // UndCC_Violation - limit is volatile
    i++;
  } while (i < limit);
}

void while_no_upper_bound()
{
  int i = 0;
  int limit = 8;
  while (limit) { // UndCC_Violation - limit not used
    i++;
  }
}
