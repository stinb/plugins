import java.util.HexFormat;
import java.util.Base64;
import java.util.*;

public class Test01 {
  private static void f00() {
    byte[] byteArray = HexFormat.of().parseHex("e04fd020ea3a6910a2d808002b30309d");
    String s = new String(byteArray);            // UNDCC_Violation - byte[] of hex type
  }

  private static void f01() {
    int x = 5;
    String s = new String(new byte[] {(byte)x}); // UNDCC_Violation - byte[] of int type
  }

  private static void f02() {
    byte[] x = new byte[100];
    var z  = x;
    String s = new String(z);                    // UNDCC_Violation - byte[] of int type
  }

  private static void f03() {
    byte[] x = new byte[100];
    String s = new String(x);                    // UNDCC_Violation - byte[] of int type
  }

  private static void f04(byte[] x) {
    String s = new String(x);                    // UNDCC_Valid - byte[] of unknown type
  }

  private static void f05(String byteStr) {
    String s = new String(byteStr.getBytes());   // UNDCC_Valid - String type
  }

  public static void main(String[] args) {

  }
}
