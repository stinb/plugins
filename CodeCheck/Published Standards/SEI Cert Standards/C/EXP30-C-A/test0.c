// SEI CERT C

extern void c(int i, int j);
int glob;

int a(void) {
  return glob + 10;
}

int b(void) {
  glob = 42;
  return glob;
}

void func_0(void) {
  c(a(), b()); // UndCC_Violation - The order of evaluation for function arguments is unspecified
}


void func_1(void) {
  int a_val, b_val;

  a_val = a();
  b_val = b();

  c(a_val, b_val);
}
