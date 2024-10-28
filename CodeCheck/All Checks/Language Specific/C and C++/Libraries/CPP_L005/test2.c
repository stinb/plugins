// Custom private functions
static double atof(const char *str) {
  return 0;
}
static int atoi(const char *str) {
  return 0;
}
static long atol(const char *str) {
  return 0;
}
static long long atoll(const char *str) {
  return 0;
}

static void f()
{
  char str[256];

  // Calls to all four functions
  atof(str);  // UndCC_Valid
  atoi(str);  // UndCC_Valid
  atol(str);  // UndCC_Valid
  atoll(str); // UndCC_Valid

  // Any reference
  int (*fn)(const char *str);
  fn = atoi;  // UndCC_Valid
  fn(str);

  // Macro expansion
  #define FN(_str) atoi(_str);
  FN(str);    // UndCC_Valid
}
