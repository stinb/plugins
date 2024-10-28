void func(unsigned int ui_a, unsigned int ui_b) {
  unsigned int usum = ui_a + ui_b; // UndCC_Violation
  /* ... */
}

void func1(unsigned int ui_a, unsigned int ui_b) {
  unsigned int usum = ui_a + ui_b; // UndCC_Valid
  if (usum < ui_a) {
    /* Handle error */
  }
  /* ... */
}

void func2(unsigned int ui_a, unsigned int ui_b) {
  unsigned int usum = ui_a + ui_b; // UndCC_Valid
  if (usum < ui_a) {
    /* Handle error */
  }
  /* ... */
}

void func3(unsigned int ui_a, unsigned int ui_b) {
  unsigned int udiff = ui_a - ui_b; // UndCC_Violation
  /* ... */
}
