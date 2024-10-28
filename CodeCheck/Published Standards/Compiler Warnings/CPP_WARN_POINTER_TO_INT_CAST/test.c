void f(void) {
  char *ptr;
  /* ... */
  unsigned int number = (unsigned int)ptr; //UndCC_Violation
  /* ... */
}

void func(unsigned int flag) {
  char *ptr;
  /* ... */
  unsigned int number = (unsigned int)ptr; //UndCC_Violation
  number = (number & 0x7fffff) | (flag << 23);
  ptr = (char *)number;
}

struct ptrflag {
  char *pointer;
  unsigned int flag : 9;
} ptrflag;
  
void func2(unsigned int flag) {
  char *ptr;
  /* ... */
  ptrflag.pointer = ptr; //UndCC_Valid
  ptrflag.flag = flag;
}