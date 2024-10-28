public class Widget2 {

  public int total; /* UndCC_Violation Primitive field declared public */

  void add() {
    if (total < Integer.MAX_VALUE) {
      total++;
      // ...
    } else {
      throw new ArithmeticException("Overflow");
    }
  }

  void remove() {
    if (total > 0) {
      total--;
      // ...
    } else {
      throw new ArithmeticException("Overflow");
    }
  }

  public static final HashMap<Integer, String> hm = new HashMap<Integer, String>(); /* UndCC_Violation Mutable field (final) declared public */


  public static final String[] items = {/* ... */}; /* UndCC_Violation Public final array declaration */
}
