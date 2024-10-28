import java.math.BigInteger;
import java.util.Base64;

public class Test00 {
  private static void f00() {
    BigInteger x = new BigInteger("530500452766");
    byte[] byteArray = x.toByteArray();
    String s = new String(byteArray);                                    // UNDCC_Violation - BigInteger type
  }

  private static void f01() {
    BigInteger x = new BigInteger("530500452766");
    String s = new String(x.toByteArray());                              // UNDCC_Violation - BigInteger type
  }

  private static void f02() {
    byte[] byteArray = new BigInteger("530500452766").toByteArray();
    String s = new String(byteArray);                                    // UNDCC_Violation - BigInteger type
  }

  private static void f03() {
    String s = new String(new BigInteger("530500452766").toByteArray()); // UNDCC_Violation - BigInteger type
  }

  private static void f04() {
    BigInteger x = new BigInteger("530500452766");
    String s = x.toString();                                             // UNDCC_Valid - String type
    byte[] byteArray = s.getBytes();
    String ns = new String(byteArray);                                   // UNDCC_Valid - byte[] from String type
  }

  private static void f05() {
    BigInteger x = new BigInteger("530500452766");
    byte[] byteArray = x.toByteArray();
    String s = Base64.getEncoder().encodeToString(byteArray);            // UNDCC_Valid - String type
  }

  public static void main(String[] args) {

  }
}
