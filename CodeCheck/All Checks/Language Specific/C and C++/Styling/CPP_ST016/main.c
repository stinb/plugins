#include <stdio.h> 
#include <stdlib.h> 

int main() 
{ 
  
  int *ptr, *ptr1, *ptr2; 
  int n, i; 
  
  // Get the number of elements for the array 
  n = 5; 
  printf("Enter number of elements: %d\n", n); 
  
  // Dynamically allocate memory using malloc() 
  ptr = (int*)malloc(n * sizeof(int)); 
  
  // Dynamically allocate memory using calloc() 
  ptr1 = (int*)calloc(n, sizeof(int)); 
  
  // Dynamically allocate memory again using calloc() 
  ptr2 = (int*)calloc(n, sizeof(int)); 
  
  // Check if the memory has been successfully allocated by malloc or not 
  if (ptr == NULL || ptr1 == NULL) { 
    printf("Memory not allocated.\n"); 
    exit(0); 
  } 
  else { 
    
    // Memory has been successfully allocated 
    printf("Memory successfully allocated using malloc.\n"); 
    
    // Free the memory 
    free(ptr); 
    printf("Malloc Memory successfully freed.\n"); 
    
    // Memory has been successfully allocated 
    printf("\nMemory successfully allocated using calloc.\n"); 
    
    // Free the memory 
    free(ptr1); 
    printf("Calloc Memory successfully freed.\n"); 
  } 
  
  // ptr2 is freed without being if-tested, flag violation
  free(ptr2);  // UndCC_Violation
  
  return 0; 
} 
