#include <stdio.h>

int main() {
  int a = 5, b = 10, result;

  // Inline assembly to add a and b UndCC_Valid
  __asm__ (
    "movl %1, %%eax;"    // Move a into eax
    "addl %2, %%eax;"    // Add b to eax
    "movl %%eax, %0;"    // Store result in result
    : "=r" (result)      // Output operand
    : "r" (a), "r" (b)   // Input operands
    : "%eax"             // Clobbered register
  );

  __asm__ ( // UndCC_Violation
    "movl %1, %%eax;"    
    "addl %2, %%eax;"    
    "movl %%eax, %0;"   
    : "=r" (result)      
    : "r" (a), "r" (b)   
    : "%eax"             
  );
  printf("Sum of %d and %d is %d\n", a, b, result);
  return 0;
}
void delay(void) {
  __asm("NOP"); // Single no-operation instruction for timing adjustment UndCC_Valid 
}

void delay1(void) {
  __asm("NOP"); // UndCC_Violation
}
