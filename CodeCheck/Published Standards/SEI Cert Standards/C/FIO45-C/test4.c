// Custom

#include <stdio.h>
#include <fcntl.h>

void fn_different_entities(const char *name_a, const char *name_b) {
  fopen(name_a, "r");
  fopen(name_b, "w");
}

void fn_same_entity(const char *name) {
  fopen(name, "r");
  fopen(name, "w"); // UndCC_Violation
}

void fn_different_string_literals() {
  fopen("hello.txt", "r");
  fopen("/dev/null", "w");
}

void fn_same_string_literals() {
  fopen("/dev/null", "r");
  fopen("/dev/null", "w"); // UndCC_Violation
}
