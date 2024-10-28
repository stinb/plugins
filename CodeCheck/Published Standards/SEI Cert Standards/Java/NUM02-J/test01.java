public class Test01 {
  private static int rollD6() {
    return (int) (Math.random() * 6);
  }

  private static void f00() {
    long result;
    result = 1 / (1 - 1);       // UNDCC_Violation
  }

  private static void f01(long num1, long num2) {
    long result;
    result = 1 / (num1 - num2); // UNDCC_Violation
  }

  private static void f02(long num1) {
    long result;
    result = 1 / (num1);        // UNDCC_Violation
  }

  private static void f03(long[] nums) {
    long result;
    result = 1 / nums[0];       // UNDCC_Violation
  }

  private static void f04(Point p) {
    long result;
    result = 1 / p.x;           // UNDCC_Violation
  }

  private static void f05() {
    long result;
    result = 1 / rollD6();      // UNDCC_Violation
  }

  private static void f06(long num1) {
    long result;
    if (num1 == 0) {
      // Handle error
    } else {
      result = 1 / (num1);      // UndCC_FalsePos - right is an expression
    }
  }

  private static void f07(long[] nums) {
    long result;
    if (nums[0] == 0) {
      // Handle error
    } else {
      result = 1 / nums[0];     // UNDCC_FalsePos - right is an expression
    }
  }

  private static void f08(Point p) {
    long result;
    if (p.x == 0) {
      // Handle error
    } else {
      result = 1 / p.x;         // UNDCC_FalsePos - right is an expression
    }
  }

  public static void main(String[] args) {
    f00();
    f01(1, 1);
    f02(0);
    f03(new long[] {0});
    f04(new Point(0, 0));
    f05();
    f06(0);
    f07(new long[] {0});
    f08(new Point(0, 0));
  }
}

public class Point {
  public long x;
  public long y;

  public Point(long x, long y) {
    x = x;
    y = y;
  }
}
