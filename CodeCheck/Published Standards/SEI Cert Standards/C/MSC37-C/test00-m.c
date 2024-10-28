
#include <string.h>
#include <stdio.h>
#include <stddef.h>

int checkpassViolation(const char *password) {
  if (strcmp(password, "pass") == 0) {
    return 1;
  }
} // UndCC_Violation

int checkpassValid(const char *password) {
  if (strcmp(password, "pass") == 0) {
    return 1;
  }
  return 0;
} // UndCC_Valid

size_t getlenViolation(const int *input, size_t maxlen, int delim) {
  for (size_t i = 0; i < maxlen; ++i) {
    if (input[i] == delim) {
      return i;
    }
  }
} // UndCC_Violation

int getlenValid(const int *input, size_t maxlen, int delim, size_t *result) {
  if (result == NULL) {
    return -1;
  }
  for (size_t i = 0; i < maxlen; ++i) {
    if (input[i] == delim) {
      *result = i;
      return 0;
    }
  }
  return -1;
} // UndCC_Valid

int main(int argc, char **argv) {
  size_t i;
} // UndCC_Valid - MSC37-C-EX1


_Noreturn void unreachable(const char *msg) {
  printf("Unreachable code reached: %s\n", msg);
  exit(1);
} // UndCC_Valid

enum E {
  One,
  Two,
  Three
};

int f(enum E e) {
  switch (e) {
    case One: return 1;
    case Two: return 2;
    case Three: return 3;
  }
  unreachable("Can never get here");
} // UndCC_Valid - MSC37-C-EX2
