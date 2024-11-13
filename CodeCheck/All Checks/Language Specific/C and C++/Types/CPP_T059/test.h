#ifndef TEST_H
#define TEST_H

#include <limits.h>
#include <signal.h>
#include <stdint.h>


typedef long int uint_64; /* 64-bit long integer */

long int bad_var4; // UndCC_Violation

char test1; // UndCC_Violation
signed char test2; // UndCC_Violation
unsigned char test3; // UndCC_Violation
short int test4; // UndCC_Violation
unsigned short int test5; // UndCC_Violation
int test6; // UndCC_Violation
unsigned int test7; // UndCC_Violation
long int test8; // UndCC_Violation
unsigned long int test9; // UndCC_Violation
long long int test10; // UndCC_Violation
unsigned long long int test11; // UndCC_Violation

intptr_t test12; // UndCC_Violation
uintptr_t test13; // UndCC_Violation
float test14;
sig_atomic_t test15;
size_t test16; // UndCC_Violation
float test17;
float test18;

#endif
