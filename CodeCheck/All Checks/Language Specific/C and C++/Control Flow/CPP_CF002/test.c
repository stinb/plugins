int main() {
  int x = 0;
  
  //Invalid
  switch (x) { // UndCC_Violation
    case 0:
      ;
    case 1: 
      ;
  }
  
  int a = 0;
  
  //Valid
  switch (a) {
    case 0:
      ;
    case 1: 
      ;
    case 2:
      ;
    default:
      ;
  }
}
