// SEI CERT C
#ifdef _WIN32
#include <Windows.h>

void formatMessageNoGoodFree()
{
  LPTSTR buf;
  DWORD n = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                          FORMAT_MESSAGE_FROM_SYSTEM |
                          FORMAT_MESSAGE_IGNORE_INSERTS, 0, GetLastError(),
                          LANG_USER_DEFAULT, (LPTSTR)&buf, 1024, 0); // UndCC_Valid
  if (n != 0) {
    /* Format and display the error to the user */

    LocalFree(buf); // UndCC_Valid
  }
}
#endif
