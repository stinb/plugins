// SEI CERT C

struct ptrflag {
  char *pointer;
  unsigned int flag : 9;
} ptrflag;

void func(unsigned int flag) {
  char *ptr;
  /* ... */
  ptrflag.pointer = ptr;
  ptrflag.flag = flag;
}
