// Custom

namespace chars {
  char c;
  wchar_t w;
}

void fn()
{
  char c = 'c';  // UndCC_Valid - Exception for =
  int i = 1;     // UndCC_Valid - Exception for =
  float f = 2.3; // UndCC_Valid - Exception for =

  // Precedence 1
  chars::c;
  chars::c++; // UndCC_Violation
  chars::w;
  chars::w++; // UndCC_Violation
}
