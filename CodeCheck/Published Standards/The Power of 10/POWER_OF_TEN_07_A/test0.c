// Normal call expressions

static void nothing(void) {

}

static void one_param(int x) {
  (void) x;
}

static void three_params(int x, int y, int z) {
  (void) x;
  (void) y;
  (void) z;
}

static int two(void) {
  return 2;
}

static int also_two(void) {
  return two();
}

static int triple_if_positive(int x) {
  return (x > 0) ? (x * 3) : x;
}

static int min_3(int x, int y, int z) {
  return (x < y && x < z) ? x :
         (y < x && y < z) ? y :
         z;
}

static void ignored_call_expressions(void) {
  int six;

  two();        // UndCC_Violation - result ignored
  two();        // UndCC_Violation - result ignored
  (void) two();
  two() + 4;
  six = two() * 3;
  {
    two();      // UndCC_Violation - result ignored
    (void) two();
  }

  six = triple_if_positive(two());
  six = triple_if_positive(two() - also_two() + two());
  six = min_3(two() * 3, two() * 3, two() * 3);

  one_param(two());
  three_params(two(), two(), two());
  three_params(0 + two() + 0, 0 + two() + 0, 0 + two() + 0);

  if (two())
    two();      // UndCC_Violation - result ignored
  else
    two();      // UndCC_Violation - result ignored

  if (six) {
    nothing();
    two();      // UndCC_Violation - result ignored
    (void) two();
  }
  if (1 && two() || 1) {
    two();      // UndCC_Violation - result ignored
  }
  if (two())
    two();      // UndCC_Violation - result ignored
  else if (two())
    two();      // UndCC_Violation - result ignored
  else
    two();      // UndCC_Violation - result ignored
  if (two()) {
    two();      // UndCC_Violation - result ignored
  }
  else if (two()) {
    two();      // UndCC_Violation - result ignored
  }
  else {
    two();      // UndCC_Violation - result ignored
  }

  for (two();;) // UndCC_Violation - result ignored
    ;
  for (;two();)
    ;
  for (;;two()) // UndCC_Violation - result ignored
    ;
  for (;;)
    two();      // UndCC_Violation - result ignored
  for (;;) {
    two();      // UndCC_Violation - result ignored
  }

  while (two())
    two();      // UndCC_Violation - result ignored
  while (two()) {
    two();      // UndCC_Violation - result ignored
  }

  do
    two();      // UndCC_Violation - result ignored
  while (two());

  do {
    two();      // UndCC_Violation - result ignored
  }
  while (two());

  six = 2;
  six <<= two();
  six -= two();
}
