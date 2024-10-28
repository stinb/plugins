int main() {
  int x;
  x = (int) 3.141592; // UNDCC_Violation
  x = (int) 'x';      // UNDCC_Violation

  return 0;
}
