// Custom

void validPath1()
{
  int a;
  int b;
}

void validPath2()
{
  if (1) {
    if (2) {
      if (3) {
        if (4) {
        }
      }
    }
  }
}


void invalidPath() // UndCC_Violation
{
  if (1) {}
  if (2) {}
  if (3) {}
  if (4) {}
  if (5) {}
  if (6) {}
  if (7) {}
  if (8) {}
  if (9) {}
}
