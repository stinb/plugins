class Test {
  private int x;
  private int y;
  private int z;
  
  int test() { // UNDCC_Violation
    if (x > 0) {
      return x;
    } else if (x < 0) {
      return y;
    } else {
      return z;
    }
  }
  
  int test1() { // UNDCC_Valid
    return x;
  }
  
  int test2() { // UNDCC_Valid
    int result;
    if (x > 0) {
      result = x;
    } else if (x < 0) {
      result = y;
    } else {
      result = z;
    }
    return result;
  }
}
