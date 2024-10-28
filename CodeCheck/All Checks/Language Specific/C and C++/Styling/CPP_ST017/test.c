#include <stdio.h>

struct student {
  int id;
  int age;
  float percentage;
};

struct student* st1;
struct student st2;

int main()
{
  int a[] = {1, 2, 3, 4};
  int b [] = {1, 2, 3, 4}; // UndCC_Violation
  int c = -1;
  int d = - 1; // UndCC_Violation
  st1 ->id = 01; // UndCC_Violation
  st1->age = 19;
  st2.id = 02;
  st2. age = 20; // UndCC_Violation
}
