public class Noncompliant3 {
  public static void main3(String argsp[]) {
    int i = 1;
    String s = Double.valueOf(i / 10000.0).toString();
    s = s.replaceFirst("[.0]*$", "");
    if (s.equals("0.0001")) {  // UndCC_Violation
      // ...
    }
  }
}
