// https://releases.llvm.org/18.1.0/tools/clang/tools/extra/docs/clang-tidy/checks/readability/non-const-parameter.html

// warning here; the declaration "const char *p" would make the function
// interface safer.
char f1(char *p) { // UndCC_Violation
  return *p;
}

// no warning; the declaration could be more const "const int * const p" but
// that does not make the function interface safer.
int f2(const int *p) { // UndCC_Valid
  return *p;
}

// no warning; making x const does not make the function interface safer
int f3(int x) { // UndCC_Valid
  return x;
}

// no warning; Technically, *p can be const ("const struct S *p"). But making
// *p const could be misleading. People might think that it's safe to pass
// const data to this function.
struct S { int *a; int *b; };
int f3(struct S *p) { // UndCC_Valid
  *(p->a) = 0;
}

// no warning; p is referenced by an lvalue.
void f4(int *p) { // UndCC_Valid
  int &x = *p;
}
