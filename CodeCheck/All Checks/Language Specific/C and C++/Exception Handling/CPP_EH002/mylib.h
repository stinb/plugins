#include <stdio.h>
#include <iostream>
#include <string>
#include <math.h>

#ifndef MYLIB_H
#define MYLIB_H

double calcSqrt(double d)
{
  return sqrt(d);
}

void throwError(int err_no) 
{
  if (err_no == -1) {
    std::cout << "An error has occurred" << std::endl; // UndCC_Violation
  }
  else {
    printf("%s \n", "No error"); // UndCC_Violation
  }
}

#endif
