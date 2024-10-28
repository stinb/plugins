#include <stdio.h>

int main() {
  char ch='B';

  /* Case 'B' is not preceded or followed by a comment explaining the intentional fall-through, flag violation */
  switch (ch)
  {
     case 'A':
        printf("CaseA");
        break;
     case 'B':  // UndCC_Violation
     case 'C':
        printf("CaseC");
        break;
     case 'D':
        // Case D is intended to fall through, I promise
     case 'E': // UndCC_Violation
        // Case E is an accident and should be fixed
     case 'F':
        // Case F is on purpose
     default:
        printf("Default");
  }

  return 0;
}
