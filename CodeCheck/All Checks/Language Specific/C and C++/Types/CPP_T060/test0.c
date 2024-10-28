// SEI CERT C

void f(void) {
  char *ptr;
  /* ... */
  unsigned int number = (unsigned int)ptr; // UndCC_Violation
  /* ... */
}
