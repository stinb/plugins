int x = 1;
int y = 2;

int my_func (int x, int y) {
  if((x = y)) {  /* UndCC_Violation */
    // ...
  }
  return x;
}
