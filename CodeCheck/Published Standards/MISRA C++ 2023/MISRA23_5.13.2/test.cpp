const char *s1 = "\1234"; // UndCC_Violation - \123 is not terminated
const char *s2 = "\x41g"; // UndCC_Violation
const char *s3 = "\x41"
                 "g";          // UndCC_Valid - terminated by end of literal
const char *s4 = "\x41\x67";   // UndCC_Valid - terminated by another escape
const char *s5 = "\u0001F600"; // UndCC_Violation(universal) - \u0001 is not terminated
const char *s6 = "\U0001F600"; // UndCC_Valid - terminated by end of literal

// MISRA C 2012 and MISRA C 2023

int c1 = '\141t';  /* UndCC_Violation                            */
int c2 = '\141\t'; /* UndCC_Valid - terminated by another escape */
