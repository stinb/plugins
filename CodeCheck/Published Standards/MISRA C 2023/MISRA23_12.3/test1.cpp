#include <iostream>

using namespace std;

int firstFunc (int num) {
  return num;
}

int secondFunc (int num1, int num2) {    /* UNDCC_Valid */
  return num1 + num2;
}

int main() {
  int a = 5;
  int b = 0;
  int c = 2;
  
  a = 2, 3, 4;                          /* UNDCC_Violation two comma operators used */
  
  b = (firstFunc(7), firstFunc(10));    /* UNDCC_Violation comma operator used */
  
  c = secondFunc(4, 5);                 /* UNDCC_Valid */
        
  cout << a;
  cout << b;
  cout << c;
  
  return 0;
}
