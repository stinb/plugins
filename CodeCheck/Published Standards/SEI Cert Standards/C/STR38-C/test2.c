// SEI CERT C

#include <string.h>
#include <wchar.h>

void func(void) {
  wchar_t wide_str1[] = L"0123456789";
  wchar_t wide_str2[] = L"0000000000";
  /* Use of proper-width function */
  wcsncpy(wide_str2, wide_str1, 10);     // UndCC_Valid

  char narrow_str1[] = "0123456789";
  char narrow_str2[] = "0000000000";
  /* Use of proper-width function */
  strncpy(narrow_str2, narrow_str1, 10); // UndCC_Valid
}
