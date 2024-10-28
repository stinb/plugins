#include <stdio.h>

static void statement_mania(void) {
  int old_value;
  int new_value;
  int total_old;
  int total_new;
  int gain; int delta_value; int delta_total;  // UndCC_Violation

  /* One statement across 2 lines */
  gain = ((old_value - new_value) /
         (total_old - total_new)) * 100.0;     // UndCC_Valid

  printf(gain);

  /* Alternate practice */
  delta_value = (old_value - new_value);

  delta_total = (total_old - total_new);

  gain = (delta_value / delta_total) * 100.0;

  printf(gain);
}
