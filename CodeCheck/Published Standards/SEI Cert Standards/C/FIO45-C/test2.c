// SEI CERT C

#if __has_include(<unistd.h>)

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void open_some_file(const char *file) {
  int fd = open(file, O_CREAT | O_EXCL | O_WRONLY);
  if (-1 != fd) {
    FILE *f = fdopen(fd, "w");
    if (NULL != f) {
      /* Write to file */

      if (fclose(f) == EOF) {
        /* Handle error */
      }
    } else {
      if (close(fd) == -1) {
        /* Handle error */
      }
    }
  }
}

#endif // __has_include(<unistd.h>)
