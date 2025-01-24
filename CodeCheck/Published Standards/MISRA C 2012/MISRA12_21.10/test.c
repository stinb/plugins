#include <wchar.h>
#include <locale.h>
#include <time.h> // UndCC_Violation

int main()
{
    // Set locale for wide-character support
    setlocale(LC_ALL, "");

    // Get current time
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);

    // Format and print the time
    wchar_t buffer[100];
    wcsftime(buffer, 100, L"%Y-%m-%d %H:%M:%S", timeinfo); // UndCC_Violation(wcsftime)
    wprintf(L"Current time: %ls\n", buffer);

    return 0;
}