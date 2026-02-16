// Custom

int f();

void g(int x)
{
  switch (x) {
    case 0:
      f();        // UndCC_Violation
      (void) f(); // UndCC_Valid
      break;
    case 1:
      (void) f(); // UndCC_Valid
      f();        // UndCC_Violation
      break;
    default:
      f();        // UndCC_Violation
      (void) f(); // UndCC_Valid
      break;
  }
}
