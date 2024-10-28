// SEI CERT C

#include <stdio.h>

// Noncompliant Code Example

void func_noncompliant(void) {
  char c_str[3] = "abc";
  printf("%s\n", c_str); // UndCC_Violation
}

// Compliant Solution

void func_compliant(void) {
  char c_str[] = "abc";
  printf("%s\n", c_str);
}
