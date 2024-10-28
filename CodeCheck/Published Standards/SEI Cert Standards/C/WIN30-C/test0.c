// SEI CERT C
#ifdef _WIN32
#include <Windows.h>
void formatMessageNoGoodFreeAndBadFree()
{
  LPTSTR buf;
  DWORD n = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                          FORMAT_MESSAGE_FROM_SYSTEM |
                          FORMAT_MESSAGE_IGNORE_INSERTS, 0, GetLastError(),
                          LANG_USER_DEFAULT, (LPTSTR)&buf, 1024, 0); // UndCC_Violation(win)
  if (n != 0) {
    /* Format and display the error to the user */

    GlobalFree(buf); // UndCC_Violation(win)
  }
}
#endif
