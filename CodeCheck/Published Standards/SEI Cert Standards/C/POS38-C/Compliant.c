#if __has_include(<unistd.h>)

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void foo(const char *filename)
{
    char c;

    pid_t pid;

    /* Open file and remember file status  */
    struct stat orig_st;
    if (lstat(filename, &orig_st) != 0)
    {
        /* handle error */
    }
    int fd = open(filename, O_RDWR);
    if (fd == -1)
    {
        /* Handle error */
    }
    struct stat new_st;
    if (fstat(fd, &new_st) != 0)
    {
        /* handle error */
    }
    if (orig_st.st_dev != new_st.st_dev ||
        orig_st.st_ino != new_st.st_ino)
    {
        /* file was tampered with while opening */
    }

    /* file is good, operate on fd */

    read(fd, &c, 1);
    printf("root process:%c\n", c);

    pid = fork();
    if (pid == -1)
    {
        /* Handle error */
    }

    if (pid == 0)
    { /*child*/
        close(fd);

        /* Reopen file, creating new file descriptor */
        fd = open(filename, O_RDONLY);
        if (fd == -1)
        {
            /* Handle error */
        }
        if (fstat(fd, &new_st) != 0)
        {
            /* handle error */
        }
        if (orig_st.st_dev != new_st.st_dev ||
            orig_st.st_ino != new_st.st_ino)
        {
            /* file was tampered with between opens */
        }

        read(fd, &c, 1);
        read(fd, &c, 1);
        printf("child:%c\n", c);
        close(fd);
    }

    else
    { /*parent*/
        read(fd, &c, 1);
        printf("parent:%c\n", c);
        close(fd);
    }
}

#endif // __has_include(<unistd.h>)
