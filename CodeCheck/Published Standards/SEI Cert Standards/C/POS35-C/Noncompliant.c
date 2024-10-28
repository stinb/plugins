#if __has_include(<pthread.h>)

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

void foo()
{

    char *filename = /* file name */ "";
    char *userbuf = /* user data */ "";
    unsigned int userlen = /* length of userbuf string */ 0;

    struct stat lstat_info;
    int fd;
    /* ... */
    if (lstat(filename, &lstat_info) == -1)
    {
        /* Handle error */
    }

    if (!S_ISLNK(lstat_info.st_mode))
    {
        fd = open(filename, O_RDWR); // UndCC_Violation(lin,mac)
        if (fd == -1)
        {
            /* Handle error */
        }
    }
    if (write(fd, userbuf, userlen) < userlen)
    {
        /* Handle error */
    }
}

#endif // __has_include(<pthread.h>)
