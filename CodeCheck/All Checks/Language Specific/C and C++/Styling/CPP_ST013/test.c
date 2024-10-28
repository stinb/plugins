int main() {
  int x = 1;
  
  // Invalid if level is set to 5 or less
  if (x == 0) { // UndCC_Violation
  } else if (x == 1) {
  } else if (x == 2) {
  } else if (x == 3) {
  } else if (x == 4) {
  } else if (x == 5) {
  } else {
  }
  
  // Valid if level is set to 4 or more
  if (x == 6)
    ;
  else if (x == 7)
    ;
  else if (x == 8)
    ;
  else if (x == 9)
    ;
  else
    ;
  
}
