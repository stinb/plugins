import java.util.*;

class Format1 {
  static Calendar c = new GregorianCalendar(1995, GregorianCalendar.MAY, 23);
  public static void main(String[] args) {
    // args[0] should contain the credit card expiration date
    // but might contain %1$tm, %1$te or %1$tY format specifiers
    System.out.printf(args[0] + " did not match! HINT: It was issued on %1$terd of some month\n", c); // UNDCC_Violation
    System.out.printf("%s did not match! HINT: It was issued on %terd of some month\n", args[0], c); // UNDCC_Valid
  }
}
