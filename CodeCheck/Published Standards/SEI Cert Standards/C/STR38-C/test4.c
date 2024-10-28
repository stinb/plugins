// SEI CERT C

#include <stdlib.h>
#include <wchar.h>

void func(void) {
  wchar_t wide_str1[] = L"0123456789";
  wchar_t *wide_str2 = (wchar_t *)malloc(
    (wcslen(wide_str1) + 1) * sizeof(wchar_t)); // UndCC_Valid
  if (wide_str2 == NULL) {
    /* Handle error */
  }
  /* ... */

  free(wide_str2);
  wide_str2 = NULL;
}
