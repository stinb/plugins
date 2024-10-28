int bar();

int main() {
  int x, y;
  if (bar()) { // UndCC_Violation
  }
  if (x + y) {} // UndCC_Violation
  
  if (x == y) {} // UndCC_Valid
  
  while(x) {} // UndCC_Violation
  
  for (int i = 0; i < 10; i++) {} // UndCC_Valid
  
  int a = x == y? 1: 0; // UndCC_Valid
  
  return 0;
}
