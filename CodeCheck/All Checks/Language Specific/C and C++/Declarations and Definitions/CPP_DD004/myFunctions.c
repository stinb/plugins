#include <stdio.h> 


int addNumbers(int a, int b); // UndCC_Violation - function prototype does not explicitly include static keyword


int main() {

   int sum;
   sum = addNumbers(4, 2);

   return 0;
}

static int addNumbers(int a, int b) {

   return a + b;

}
