#include <fcntl.h>

#ifdef _WIN32
    typedef int mode_t;
#endif

void func(const char *ms, mode_t perms)
{
    /* ... */
    int fd;
    fd = open(ms, O_CREAT | O_EXCL | O_WRONLY | O_TRUNC, perms);
    if (fd == -1)
    {
        /* Handle error */
    }
}
