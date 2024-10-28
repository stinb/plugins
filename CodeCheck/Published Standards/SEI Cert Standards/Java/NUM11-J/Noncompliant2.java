public class Noncompliant2 {
  public static void main2(String argsp[]) {
    int i = 1;
    String s = Double.valueOf(i / 1000.0).toString();
    s = s.replaceFirst("[.0]*$", "");
    if (s.equals("0.001")) {  // UndCC_Violation
      // ...
    }
  }
}
