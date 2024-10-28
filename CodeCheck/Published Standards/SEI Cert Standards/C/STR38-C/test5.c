// Custom

#include <wchar.h>

void func_c(char c) {}
void func_w(wchar_t w) {}

void func_c_str(char *str) {}
void func_w_str(wchar_t *str) {}

void func_c_str_w_str(char *c_str, wchar_t *w_str) {}
void func_w_str_c_str(wchar_t *w_str, char *c_str) {}

void func_c_str_int_w_str(char *c_str, int x, wchar_t *w_str) {}
void func_w_str_int_c_str(wchar_t *w_str, int x, char *c_str) {}

void func(void) {
  char c;
  wchar_t w;

  char *c_str;
  wchar_t *w_str;

  func_c(c); // UndCC_Valid
  func_c(w); // UndCC_Valid

  func_w(c); // UndCC_Valid
  func_w(w); // UndCC_Valid

  func_c_str(c_str); // UndCC_Valid
  func_c_str(w_str); // UndCC_Violation

  func_w_str(c_str); // UndCC_Violation
  func_w_str(w_str); // UndCC_Valid

  func_c_str_w_str(c_str, c_str); // UndCC_Violation
  func_c_str_w_str(c_str, w_str); // UndCC_Valid
  func_c_str_w_str(w_str, c_str); // UndCC_Violation
  func_c_str_w_str(w_str, w_str); // UndCC_Violation

  func_w_str_c_str(c_str, c_str); // UndCC_Violation
  func_w_str_c_str(c_str, w_str); // UndCC_Violation
  func_w_str_c_str(w_str, c_str); // UndCC_Valid
  func_w_str_c_str(w_str, w_str); // UndCC_Violation

  func_c_str_w_str("char",  "char");  // UndCC_Violation
  func_c_str_w_str("char",  L"wide"); // UndCC_Valid
  func_c_str_w_str(L"wide", "char");  // UndCC_Violation
  func_c_str_w_str(L"wide", L"wide"); // UndCC_Violation

  func_w_str_c_str("char",  "char");  // UndCC_Violation
  func_w_str_c_str("char",  L"wide"); // UndCC_Violation
  func_w_str_c_str(L"wide", "char");  // UndCC_Valid
  func_w_str_c_str(L"wide", L"wide"); // UndCC_Violation

  func_c_str_int_w_str(c_str, 0, c_str); // UndCC_Violation
  func_c_str_int_w_str(c_str, 1, w_str); // UndCC_Valid
  func_c_str_int_w_str(w_str, 2, c_str); // UndCC_Violation
  func_c_str_int_w_str(w_str, 3, w_str); // UndCC_Violation

  func_w_str_int_c_str(c_str, 4, c_str); // UndCC_Violation
  func_w_str_int_c_str(c_str, 5, w_str); // UndCC_Violation
  func_w_str_int_c_str(w_str, 6, c_str); // UndCC_Valid
  func_w_str_int_c_str(w_str, 7, w_str); // UndCC_Violation
}
