#include <stdint.h>
#include <cstdio>
uint16_t f ( uint16_t *p ) // UndCC_Violation
{
  return *p;
}
uint16_t g ( const uint16_t *p ); // UndCC_Valid
void h ( const uint16_t *p ) // UndCC_Valid
{
  //*p = 0;
}
void process_data(const int *ptr) { // UndCC_Valid
  int x = *ptr;
  //use(x);
}
int main(int argc, char* argv[]) { // UndCC_Valid arcv in main is an exception
  // ...
}
void print_name(void) {
  char *name = "Alice";     // UndCC_Violation
}
void print_str(char *str) {   // UndCC_Violation 
  printf("%s\n", str);
}
void increment_values(int *arr, int size) { // UndCC_Valid arr is modified
  for (int i = 0; i < size; i++) {
    arr[i] += 1; // Modifying the data through the pointer
  }
}
