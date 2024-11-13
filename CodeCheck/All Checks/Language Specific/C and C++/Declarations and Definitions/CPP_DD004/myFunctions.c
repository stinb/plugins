#include <stdio.h> 


int addNumbers(int a, int b); 


int main() {

   int sum;
   sum = addNumbers(4, 2);

   return 0;
}

static int addNumbers(int a, int b) {  // UndCC_Violation - function prototype does not explicitly include static keyword

   return a + b;

}
