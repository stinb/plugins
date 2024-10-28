// Custom
#ifdef _WIN32
#include <Windows.h>

void formatMessageNoGoodFreeOfObject()
{
  LPTSTR buf;
  DWORD n = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                          FORMAT_MESSAGE_FROM_SYSTEM |
                          FORMAT_MESSAGE_IGNORE_INSERTS, 0, GetLastError(),
                          LANG_USER_DEFAULT, (LPTSTR)&buf, 1024, 0); // UndCC_Violation(win)
}

void formatMessageNoGoodFreeOfParameter(LPTSTR buf)
{
  DWORD n = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                          FORMAT_MESSAGE_FROM_SYSTEM |
                          FORMAT_MESSAGE_IGNORE_INSERTS, 0, GetLastError(),
                          LANG_USER_DEFAULT, (LPTSTR)&buf, 1024, 0); // UndCC_Violation(win)
}
#endif

void mallocNoGoodFreeObject()
{
  int *buf = malloc(4 * sizeof(int)); // UndCC_Violation
}

void mallocNoGoodFreeAndBadFreeObject()
{
  int *buf = malloc(4 * sizeof(int)); // UndCC_Violation
  LocalFree(buf);                     // UndCC_Violation(win)
}

void mallocGoodFreeObject()
{
  int *buf = malloc(4 * sizeof(int)); // UndCC_Valid
  free(buf);                          // UndCC_Valid
}

void mallocNoGoodFreeParameter(int *buf)
{
  buf = malloc(4 * sizeof(int)); // UndCC_Violation
}

void mallocNoGoodFreeAndBadFreeParameter(int *buf)
{
  buf = malloc(4 * sizeof(int)); // UndCC_Violation
  LocalFree(buf);                // UndCC_Violation(win)
}

void mallocGoodFreeParameter(int *buf)
{
  buf = malloc(4 * sizeof(int)); // UndCC_Valid
  free(buf);                     // UndCC_Valid
}
