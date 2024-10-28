#if __has_include(<unistd.h>)

#include <unistd.h>

void foo()
{

    /* Drop superuser privileges in incorrect order */

    if (setuid(getuid()) == -1)
    {
        /* handle error condition */
    }
    if (setgid(getgid()) == -1) // UndCC_Violation(lin,mac)
    {
        /* handle error condition */
    }

    /* It is still possible to regain group privileges due to
     * incorrect relinquishment order */
}

#endif // __has_include(<unistd.h>)
