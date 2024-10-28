#include <stdio.h>

int main() {
  if (1) {
  }
  
  if (1)
  { // UndCC_Violation
  }
  
  if (1)
    { // UndCC_Violation
    } // UndCC_Violation
    
  if (1) {
    } // UndCC_Violation
    
  if (0) {
  }
}

int func() {
  if (1)
  { // UndCC_Violation
  }
  
  if (1)
    { // UndCC_Violation
    } // UndCC_Violation
  
  if (1) {
  }
  
  if (1) {
    } // UndCC_Violation
  
  if (0)
  { // UndCC_Violation
  }
}

int func0() {
  if (1)
    { // UndCC_Violation
    } // UndCC_Violation
  
  if (1)
  { // UndCC_Violation
  }
  
  if (1) {
  }
  
  if (1) {
    } // UndCC_Violation
  
  if (0)
    { // UndCC_Violation
    } // UndCC_Violation
}
