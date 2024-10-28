public class Test00 {
  private static void f00() {
    long result;
    result = 1 / 0;         // UNDCC_Violation
  }

  private static void f01() {
    long num1, result;
    num1 = 1;
    result = num1 / 0;      // UNDCC_Violation
  }

  private static void f02() {
    long num1, result;
    num1 = 0;
    result = 1 / num1;      // UNDCC_Violation
  }

  private static void f03() {
    long num1, num2, result;
    num1 = 1;
    num2 = 0;

    result = num1 % num2;   // UNDCC_Violation
  }

  private static void f04(long num1, long num2) {
    long result;

    result = num1 / num2;   // UNDCC_Violation
  }

  private static void f05() {
    long num1, num2, result;
    num1 = 1;
    num2 = 0;

    result = num1 % num2;   // UNDCC_Violation
  }

  private static void f06() {
    long num1, num2, result;
    num1 = 1;
    num2 = 0;

    if (num2 == 0) {
      // Handle error
    } else {
      result = num1 / num2; // UNDCC_Valid - checked
    }
  }

  private static void f07() {
    long num1, num2, result;
    num1 = 1;
    num2 = 0;

    if (num2 == 0) {
      // Handle error
    } else {
      result = num1 % num2; // UNDCC_Valid - checked
    }
  }

  private static void f08() {
    long num1, num2, result;
    num1 = 1;
    num2 = 2;

    if (num1 % num2 == 0) { // UNDCC_Valid - checked

    }
  }

  public static void main(String[] args) {
    f00();
    f01();
    f02();
    f03();
    f04(1, 2);
    f05();
    f06();
    f07();
    f08();
  }
}
