// MISRA C 2023

#include <stdio.h>

void f()
{
  FILE *pf1;
  FILE *pf2;
  FILE f3;
  pf2 = pf1;                /* UndCC_Valid */
   f3 = *pf2;               /* UndCC_Violation */

#ifdef _WIN32
  pf1->_Placeholder = 0;    /* UndCC_Violation(win) */
#elif defined(__linux__)
  pf1->_fileno = 0;         /* UndCC_Violation(lin) */
#elif defined(__APPLE__)
  pf1->_file = 0;           /* UndCC_Violation(mac) */
#endif
}
