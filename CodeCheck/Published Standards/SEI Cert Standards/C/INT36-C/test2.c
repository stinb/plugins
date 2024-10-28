// SEI CERT C

void func(unsigned int flag) {
  char *ptr;
  /* ... */
  unsigned int number = (unsigned int)ptr; // UndCC_Violation
  number = (number & 0x7fffff) | (flag << 23);
  ptr = (char *)number; // UndCC_Violation
}
