// SEI CERT C

unsigned int *g(void) {
  unsigned int *ptr = 0xdeadbeef; // UndCC_Violation
  /* ... */
  return ptr;
}
