public class Test02 {
  public static void main(String[] args) {
    int x;
    long y;
    final int INT_MIN = -2147483648;
    final long LONG_MIN = -9223372036854775808L;

    x = 2147483647 + 1;          // UndCC_Violation
    System.out.println(x);

    x = -2147483648 - 1;         // UndCC_Violation
    System.out.println(x);

    x = 2147483647 * 2;          // UndCC_Violation
    System.out.println(x);

    x = - -2147483648;           // UndCC_Violation
    System.out.println(x);

    y = - -9223372036854775808L; // UndCC_Violation
    System.out.println(y);

    x = -INT_MIN;                // UndCC_Violation
    System.out.println(x);

    y = -LONG_MIN;               // UndCC_Violation
    System.out.println(y);

    x = 2147483647 * 2;          // UndCC_Violation
    System.out.println(x);

    x = Math.abs(-2147483648);   // UndCC_Violation
    System.out.println(x);

    x = Math.abs(-2147483647);   // UndCC_Valid
    System.out.println(x);
  }
}
