public class Test03 {
  public static int fun(int x) {
    return x / 2;
  }

  public static void f00() {
    int result;
    final int num1 = 0;
    result = 1 / num1;               // UNDCC_Violation
  }

  public static void f01() {
    int result;
    final int num1 = 1;
    result = 1 / num1;               // UNDCC_Valid
  }

  public static void f02() {
    int result;
    result = (int) 1.0 / 0;          // UNDCC_Violation
  }

  public static void f03() {
    int result;
    result = (int) (Math.PI / 0);    // UNDCC_Valid
  }

  public static void f04() {
    int result;
    result = (int) Math.PI / 0;      // UNDCC_FalseNeg - left is an expression
  }

  public static void f05() {
    int a = 1;
    int b = 0;
    fun(a / b);                      // UNDCC_Violation
  }

  public static void f06() {
    int a = 1;
    int b = 0;
    double c = (double)a / b;        // UNDCC_Valid
  }

  public static void f07() {
    int a = 1;
    int b = 0;
    int c = (int) ((double) a / b);  // UNDCC_Valid
  }

  public static void main(String[] args) {
    // f00();
    f01();
    // f02();
    f03();
    // f04();
    // f05();
    f06();
    f07();
  }
}
