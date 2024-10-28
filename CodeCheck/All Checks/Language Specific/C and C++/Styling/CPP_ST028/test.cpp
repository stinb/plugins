#include <iostream>

struct Books { // UndCC_Violation
  char  title[50];
  char  author[50];
  char  subject[100];
  int   book_id;
} book;

struct t_Animals {
  char  name[50];
  char  species[50];
} animal;

typedef unsigned int uint; // UndCC_Violation
typedef unsigned int t_length;

int main() {
  uint i, j;
  
  return 0;
}
