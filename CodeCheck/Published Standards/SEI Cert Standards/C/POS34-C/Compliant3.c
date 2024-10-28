#include <stdlib.h>

int func(const char *var) {
  return setenv("TEST", var, 1);
}
