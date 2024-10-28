int main() {
  int x = 1;
  int y = 2;
  
  bool a = 0;
  
  switch (x == 0){} /* UNDCC_Violation result of x == 0 is of bool type */
  switch (a) {} /* UNDCC_Violation a is of bool type */
  
  switch (x < y) {} /* UNDCC_Violation result of x < y is of bool type */
  
  switch (x) {} /* UNDCC_Valid */
}
