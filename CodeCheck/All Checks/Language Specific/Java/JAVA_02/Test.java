class Test {
  private String x; // UNDCC_Violation
  private double y; // UNDCC_Valid
  private char z; // UNDCC_Violation
  
  double test() {
    return y;
  }
}
