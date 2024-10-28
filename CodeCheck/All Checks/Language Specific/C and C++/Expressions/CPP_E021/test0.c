// MISRA C 2012 and MISRA C 2023

const char *s1 = "\x41g";    /* UndCC_Violation                            */
const char *s2 = "\x41" "g"; /* UndCC_Valid - terminated by end of literal */
const char *s3 = "\x41\x67"; /* UndCC_Valid - terminated by another escape */
int c1 = '\141t';            /* UndCC_Violation                            */
int c2 = '\141\t';           /* UndCC_Valid - terminated by another escape */
