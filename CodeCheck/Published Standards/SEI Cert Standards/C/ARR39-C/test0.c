// SEI CERT C

enum { INTBUFSIZE = 80 };

extern int getdata(void);
int buf[INTBUFSIZE];

void func(void) {
  int *buf_ptr = buf;

  while (buf_ptr < (buf + sizeof(buf))) { // UndCC_Violation - sizeof(buf) is scaled
    *buf_ptr++ = getdata();
  }
}
