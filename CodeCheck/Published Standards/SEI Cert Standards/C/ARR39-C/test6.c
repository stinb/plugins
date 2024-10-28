// Custom

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#ifndef offsetof
#define offsetof __builtin_offsetof
#endif

enum { INTBUFSIZE = 80 };
int buf[INTBUFSIZE];
int *buf_ptr = buf;

struct big {
  unsigned long long ull_a;
  unsigned long long ull_b;
  unsigned long long ull_c;
  int si_e;
  int si_f;
};
size_t skip = offsetof(struct big, ull_b);

enum { WCHAR_BUF = 128 };
const wchar_t ERROR_PREFIX[7] = L"Error: ";
wchar_t error_msg[WCHAR_BUF];

int  ints[64];
long longs[64];
char str[64];

void print_ptr_and_offset(void *ptr, void *sum) {
  printf("pointer:          %p\n", ptr);
  printf("pointer + offset: %p\n", sum);
  printf("offset:           %zu\n", sum - ptr);
}

int main() {
  void *sum;

  // test0
  sum = buf + sizeof(buf);                               // UndCC_Violation
  print_ptr_and_offset(buf, sum);
  printf("\n");

  // test1
  sum = buf + INTBUFSIZE;                                // UndCC_Valid
  print_ptr_and_offset(buf, sum);
  printf("\n");

  // test2
  struct big *s = malloc(sizeof(struct big));
  sum = s + skip;                                        // UndCC_Violation
  print_ptr_and_offset(s, sum);
  printf("\n");

  // test3
  unsigned char *ptr = malloc(sizeof(struct big));
  sum = ptr + skip;                                      // UndCC_Valid
  print_ptr_and_offset(ptr, sum);
  printf("\n");

  // test4
  wcscpy(error_msg, ERROR_PREFIX);
  sum = error_msg + wcslen(error_msg) * sizeof(wchar_t); // UndCC_Violation
  print_ptr_and_offset(error_msg, sum);
  printf("\n");

  // test5
  const size_t prefix_len = wcslen(ERROR_PREFIX);
  sum = error_msg + prefix_len;                          // UndCC_Valid
  print_ptr_and_offset(error_msg, sum);
  printf("\n");

  // exception: number of elements in array
  sum = buf + sizeof(buf) / sizeof(buf[0]);              // UndCC_Valid
  print_ptr_and_offset(buf, sum);
  printf("\n");

  // custom: alignof
  sum = buf + _Alignof(buf) * INTBUFSIZE;                // UndCC_Violation
  print_ptr_and_offset(buf, sum);
  printf("\n");

  // custom: macro
  #define sizeof_buf sizeof(buf)
  sum = buf + sizeof_buf;                                // UndCC_Violation
  print_ptr_and_offset(buf, sum);
  printf("\n");

  // custom: different types
  sum = ints + sizeof(longs);                            // UndCC_Violation
  print_ptr_and_offset(ints, sum);
  printf("\n");

  // custom: char *
  sum = str + sizeof(longs);                             // UndCC_Valid
  print_ptr_and_offset(str, sum);
  printf("\n");

  // custom: cast to char *
  sum = (char *) ints + sizeof(longs);                   // UndCC_Valid
  print_ptr_and_offset(ints, sum);
  printf("\n");

  // custom: cast from char *
  sum = (int *) str + sizeof(longs);                     // UndCC_Violation
  print_ptr_and_offset(str, sum);
  printf("\n");

  return 0;
}
