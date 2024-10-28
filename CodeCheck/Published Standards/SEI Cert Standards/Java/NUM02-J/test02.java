public class Test02 {
  public static void f00() {
    Float num1, result;
    num1 = (float) 1.0;
    result = num1 / 0;            // UNDCC_Valid - left is a float
  }

  public static void f01() {
    float num1, result;
    num1 = 0;
    result = 1 / num1;            // UNDCC_Valid - right is a float
  }

  public static void f02() {
    Double num1, num2, result;
    num1 = (double) 1;
    num2 = (double) 0;
    result = num1 / num2;         // UNDCC_Valid - left & right are Double
  }

  public static void f03() {
    int num1;
    double num2, result;
    num1 = 1;
    num2 = 0;
    result = num1 / num2;         // UNDCC_Valid - right is a double
  }

  public static void f04() {
    double num1, result;
    int num2;
    num1 = 1;
    num2 = 0;
    result = num1 / num2;         // UNDCC_Valid - left is a double
  }

  public static void f05() {
    int num1;
    double result;
    num1 = 1;
    result = num1 / 0.0;          // UNDCC_Valid - right is a double literal
  }

  public static void f06() {
    Integer num1, result;
    num1 = 0;
    result = (int) (1.0 / num1);  // UNDCC_Valid - left is a double literal
  }

  public static void f07() {
    double result;
    result = 1 / (double) 0;      // UNDCC_Valid - right is cast as a double
  }

  public static void f08() {
    double result;
    result = (float) 1 / 0;       // UNDCC_Valid - left is cast as a float
  }

  public static void main(String[] args) {
    f00();
    f01();
    f02();
    f03();
    f04();
    f05();
    f06();
    f07();
    f08();
  }
}
