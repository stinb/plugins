#include <stdio.h>
#include <stdbool.h>

int main() {
  bool x = 1;
  bool y = true; // UndCC_Violation
  if(x){
    printf("x is true.");
  }
  else{
    printf("x is false.");
  }
  if(y){
    printf("y is true.");
  }
  else{
    printf("y is false.");
  }
}
