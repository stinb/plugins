public class Test03 {
  public static void main(String[] args) {
    int i;
    long l;
    final int INT_MIN = -2147483648;
    final int INT_MAX = 2147483647;
    final long LONG_MIN = -9223372036854775808L;
    final long LONG_MAX = 9223372036854775807L;

    i = INT_MAX;
    l = (long) i * i;        // UndCC_Valid - (integer -> long) * (integer -> long)
    System.out.println(l);

    l = i * (long) i;        // UndCC_Valid - (integer -> long) * (integer -> long)
    System.out.println(l);

    l = (long) i * (long) i; // UndCC_Valid - (integer -> long) * (integer -> long)
    System.out.println(l);

    l = (int) 2147483647 * 2147483647;       // UndCC_Violation - (integer -> integer) * (integer -> integer)
    System.out.println(l);

    l = 2147483647 * (int) 2147483647;       // UndCC_Violation - (integer -> integer) * (integer -> integer)
    System.out.println(l);

    l = (int) 2147483647 * (int) 2147483647; // UndCC_Violation - (integer -> integer) * (integer -> integer)
    System.out.println(l);

    l = (long) 2147483647 * 2147483647;        // UndCC_Valid - (integer -> long) * (integer -> long)
    System.out.println(l);

    l = 2147483647 * (long) 2147483647;        // UndCC_Valid - (integer -> long) * (integer -> long)
    System.out.println(l);

    l = (long) 2147483647 * (long) 2147483647; // UndCC_Valid - (integer -> long) * (integer -> long)
    System.out.println(l);

    l = LONG_MAX;
    l = (long) i * l;        // UndCC_Violation - (integer -> long) * long
    System.out.println(l);

    l = LONG_MAX;
    l = l * (long) i;        // UndCC_Violation - long * (integer -> long)
    System.out.println(l);

    i = INT_MIN;
    l = Math.abs((long) i); // UndCC_Valid - (integer -> long)
    System.out.println(l);
  }
}
