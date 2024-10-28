static void multiple_declarations(int a, int, b)
{
  int c;
  int d, e; // UndCC_Violation
}

static void multiple_statements(int a)
{
  if (a > 0) {
    ;
  }
  else if (a < 0) {
    ;
  }
  else { ; } // UndCC_Violation

  switch (a) {
    case 1:
      a *= 2;
      break;
    default:
      a *= 3; break; // UndCC_Violation
  }

  for (int i = 0; i < 10; i++);      // UndCC_Violation
  for (int i = 0; i < 10; i++) { ; } // UndCC_Violation
  for (int i = 0; i < 10; i++)
    ;
  for (int i = 0; i < 10; i++)
    { ; }

  while (a < 20) {
    a += 3; if (a < 0) // UndCC_Violation
      break;
  }

  do {
    a += 3;
    if (a < 0) break; // UndCC_Violation
  } while (a < 20);
}
