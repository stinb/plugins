#if __has_include(<unistd.h>)

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void foo(const char *filename)
{
    char c;
    pid_t pid;

    int fd = open(filename, O_RDWR);
    if (fd == -1)
    {
        /* Handle error */
    }
    read(fd, &c, 1);
    printf("root process:%c\n", c);

    pid = fork();
    if (pid == -1)
    {
        /* Handle error */
    }

    if (pid == 0)
    {                    /*child*/
        read(fd, &c, 1); // UndCC_Violation(lin, mac)
        printf("child:%c\n", c);
    }
    else
    {                    /*parent*/
        read(fd, &c, 1); // UndCC_Violation(lin, mac)
        printf("parent:%c\n", c);
    }
}

#endif // __has_include(<unistd.h>)
