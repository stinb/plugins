// SEI CERT C

#include <stdio.h>

int arr[64];


void func_0(int i) {
  i = i + 1;
  arr[i] = i;
}


void func_1(int i) {
  /* i is modified twice between sequence points */
  i = ++i + 1; // UndCC_Violation

  /* i is read other than to determine the value to be stored */
  arr[i++] = i; // UndCC_Violation
}


void func_2(int i, int *b) {
  int a = i + b[++i]; // UndCC_Violation - i is evaluated twice without an intervening sequence point
  printf("%d, %d", a, i);
}


void func_3(int i, int *b) {
  int a;
  ++i;
  a = i + b[i];
  printf("%d, %d", a, i);
}


void func_4(int i, int *b) {
  int a = i + b[i + 1];
  ++i;
  printf("%d, %d", a, i);
}


extern void func(int i, int j);

void f_0(int i) {
  func(i++, i); // UndCC_Violation - there is no sequence point between the argument expressions
}


void f_1(int i) {
  i++;
  func(i, i);
}


void f_2(int i) {
  int j = i++;
  func(j, i);
}
