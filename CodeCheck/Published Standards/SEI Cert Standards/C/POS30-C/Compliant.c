typedef signed int ssize_t;

void foo()
{

  enum { BUFFERSIZE = 1024 };
  char buf[BUFFERSIZE];
  ssize_t len = readlink("/usr/bin/perl", buf, sizeof(buf)-1); // UndCC_Valid

  if (len != -1) {
    buf[len] = '\0';
  }
  else {
    /* handle error condition */
  }
}
