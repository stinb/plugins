
int main() {
  int flag = 1;
  for (int i = 0; (i < 5) && (flag == 1); i++ ) { /* UndCC_Violation - loop counter i is altered within the loop */
    /* ... */
    flag = 0;   /* UndCC_Valid - allows early termination of loop */
    i = i + 3;
  }
  return 0;
}
