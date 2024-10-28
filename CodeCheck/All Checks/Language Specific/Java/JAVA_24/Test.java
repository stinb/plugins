class Test {
  public static void main(String[] args) {
    int a = 0; // UNDCC_Valid
  int b = 0; // UNDCC_Violation

    if (false) {
    int a = 0; // UNDCC_Violation
    }
  }
}

