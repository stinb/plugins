// SEI CERT C

#include <sys/stat.h>
#include <fcntl.h>

int open_regular_file(char *filename, int flags) {
  struct stat lstat_info;
  struct stat fstat_info;
  int f;

  if (lstat(filename, &lstat_info) == -1) {
    /* File does not exist, handle error */
  }

  if (!S_ISREG(lstat_info.st_mode)) {
    /* File is not a regular file, handle error */
  }

  if ((f = open(filename, flags)) == -1) {
    /* File has disappeared, handle error */
  }

  if (fstat(f, &fstat_info) == -1) {
    /* Handle error */
  }

  if (lstat_info.st_ino != fstat_info.st_ino  ||
      lstat_info.st_dev != fstat_info.st_dev) {
    /* Open file is not the expected regular file, handle error */
  }

  /* f is the expected regular open file */
  return f;
}
