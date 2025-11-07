// Custom

#include <stdint.h>
#include <string.h>

char buf1[5] = "12345";
char buf2[10] = "1234567890";

void overflow(void)
{
  memcmp(buf1, buf2, UINT64_MAX + 1); // UndCC_Violation - size must be positive but it is 0
  memcmp(buf1, buf2, UINT64_MAX + 2); // UndCC_Valid
  memcmp(buf1, buf2, UINT64_MAX + 3); // UndCC_Valid
  memcmp(buf1, buf2, UINT64_MAX + 4); // UndCC_Valid
  memcmp(buf1, buf2, UINT64_MAX + 5); // UndCC_Valid
  memcmp(buf1, buf2, UINT64_MAX + 6); // UndCC_Valid
  memcmp(buf1, buf2, UINT64_MAX + 7); // UndCC_Violation - size must be <= 5 but it is 6
}

void functions(void)
{
  memchr(buf1, '\0', 5); // UndCC_Valid
  memchr(buf1, '\0', 6); // UndCC_Violation

  memcmp(buf1, buf2, 5); // UndCC_Valid
  memcmp(buf1, buf2, 6); // UndCC_Violation

  memcpy(buf1, buf2, 5); // UndCC_Valid
  memcpy(buf1, buf2, 6); // UndCC_Violation

  memmove(buf1, buf2, 5); // UndCC_Valid
  memmove(buf1, buf2, 6); // UndCC_Violation

  memset(buf1, '\0', 5); // UndCC_Valid
  memset(buf1, '\0', 6); // UndCC_Violation

  strncat(buf1, buf2, 5); // UndCC_Valid
  strncat(buf1, buf2, 6); // UndCC_Violation

  strncmp(buf1, buf2, 5); // UndCC_Valid
  strncmp(buf1, buf2, 6); // UndCC_Violation

  strncpy(buf1, buf2, 5); // UndCC_Valid
  strncpy(buf1, buf2, 6); // UndCC_Violation

  strxfrm(buf1, buf2, 5); // UndCC_Valid
  strxfrm(buf1, buf2, 6); // UndCC_Violation
}
