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
    struct stat fstat_info;
    int fd;
    /* ... */
    if (lstat(filename, &lstat_info) == -1)
    {
        /* handle error */
    }

    fd = open(filename, O_RDWR);
    if (fd == -1)
    {
        /* handle error */
    }

    if (fstat(fd, &fstat_info) == -1)
    {
        /* handle error */
    }

    if (lstat_info.st_mode == fstat_info.st_mode &&
        lstat_info.st_ino == fstat_info.st_ino &&
        lstat_info.st_dev == fstat_info.st_dev)
    {
        if (write(fd, userbuf, userlen) < userlen)
        {
            /* Handle Error */
        }
    }
}

#endif // __has_include(<pthread.h>)
