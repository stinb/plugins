// Custom

#include <cstdio>

int  ints[64];
long longs[64];
char str[64];

void print_ptr_and_offset(void *ptr, void *sum) {
  printf("pointer:          %p\n", ptr);
  printf("pointer + offset: %p\n", sum);
  printf("offset:           %zu\n", (char *) sum - (char *) ptr);
}

int main() {
  void *sum;

  // custom: cast to char *
  sum = reinterpret_cast<char *>(ints) + sizeof(longs); // UndCC_Valid
  print_ptr_and_offset(ints, sum);
  printf("\n");

  // custom: cast from char *
  sum = reinterpret_cast<int *>(str) + sizeof(longs);  // UndCC_Violation
  print_ptr_and_offset(str, sum);
  printf("\n");

  return 0;
}
