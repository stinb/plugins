
#include <stdio.h>

void foo() {
  float i;   
  
  i = 10.1;   
  
  if (i==1){ //UndCC_Violation
  i++;}
  
  int j;
  j = 10;
  
  if(j==10){
      j++;
  };
  
}


