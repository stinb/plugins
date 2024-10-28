public class Test {
  int a;
  int b;

  public String test(int aa, int bb) { // UNDCC_Violation
    return aa + " " + bb;
  }
  public String test1() { // UNDCC_Violation
    return a + " " + b;
  }

  public String test2(int aa) { // UNDCC_Violation
    String bb = "Test: "+ aa;
    return bb;
  }
  public String test3() { // UNDCC_Violation
    String aa = a + " " + b;
    return aa;
  }
  public String toString(int aa) { // UNDCC_Valid
    String bb = "Test: " + aa;
    return bb;
  }
}

