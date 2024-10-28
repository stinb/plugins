public class Test01 {
  public static void main(String[] args) {
    int x;

    x = Integer.MAX_VALUE + 1;       // UndCC_Violation
    x = x + 1;                       // UndCC_Violation
    x = x += 1;                      // UndCC_Violation
    x++;                             // UndCC_Violation
    System.out.println(x);

    x = Integer.MIN_VALUE - 1;       // UndCC_Violation
    x = x - 1;                       // UndCC_Violation
    x = x -= 1;                      // UndCC_Violation
    x--;                             // UndCC_Violation
    System.out.println(x);

    x = Integer.MAX_VALUE * 2;       // UndCC_Violation
    x = x * 2;                       // UndCC_Violation
    x = x *= 2;                      // UndCC_Violation
    System.out.println(x);

    x = -Integer.MIN_VALUE;          // UndCC_Violation
    x = -x;                          // UndCC_Violation
    System.out.println(x);

    x = Integer.MIN_VALUE / 2;       // UndCC_Violation
    x = x / 2;                       // UndCC_Violation
    x = x /= 2;                      // UndCC_Violation
    System.out.println(x);

    x = Integer.MIN_VALUE;
    x = Math.abs(x);                 // UndCC_Violation
    x = Math.abs(Integer.MIN_VALUE); // UndCC_Violation
    System.out.println(x);
  }
}
