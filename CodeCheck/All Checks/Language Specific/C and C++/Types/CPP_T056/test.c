int main () {
  unsigned int u_int;
  int s_int;
  
  unsigned int j = u_int + u_int; /* UNDCC_Valid */
  
  unsigned int k = s_int + u_int; /* UNDCC_Violation */
  
  unsigned int a = (unsigned int)s_int + s_int; /* UNDCC_Valid */
  
  int b = (int) u_int + s_int; /* UNDCC_Valid */
  
  int c = u_int + s_int; /* UNDCC_Violation */
}
