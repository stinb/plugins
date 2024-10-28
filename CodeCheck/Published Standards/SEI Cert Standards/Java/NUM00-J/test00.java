import java.math.BigInteger;
import java.util.concurrent.atomic.AtomicInteger;

public class Test00 {
  static final int safeAdd(int left, int right) {
    if (right > 0 ? left > Integer.MAX_VALUE - right
                  : left < Integer.MIN_VALUE - right) {
      throw new ArithmeticException("Integer overflow");
    }
    return left + right;                                             // UndCC_Valid
  }

  static final int safeSubtract(int left, int right) {
    if (right > 0 ? left < Integer.MIN_VALUE + right
                  : left > Integer.MAX_VALUE + right) {
      throw new ArithmeticException("Integer overflow");
    }
    return left - right;                                             // UndCC_Valid
  }

  static final int safeMultiply(int left, int right) {
    if (right > 0 ? left > Integer.MAX_VALUE/right
                    || left < Integer.MIN_VALUE/right
                  : (right < -1 ? left > Integer.MIN_VALUE/right
                                  || left < Integer.MAX_VALUE/right
                                : right == -1
                                  && left == Integer.MIN_VALUE) ) {
      throw new ArithmeticException("Integer overflow");
    }
    return left * right;                                             // UndCC_Valid
  }

  static final int safeDivide(int left, int right) {
    if ((left == Integer.MIN_VALUE) && (right == -1)) {
      throw new ArithmeticException("Integer overflow");
    }
    return left / right;                                             // UndCC_Valid
  }

  static final int safeNegate(int a) {
    if (a == Integer.MIN_VALUE) {
      throw new ArithmeticException("Integer overflow");
    }
    return -a;                                                       // UndCC_Valid
  }

  static final int safeAbs(int a) {
    if (a == Integer.MIN_VALUE) {
      throw new ArithmeticException("Integer overflow");
    }
    return Math.abs(a);                                              // UndCC_Valid
  }

  public static int multAccum0(int oldAcc, int newVal, int scale) {
    return oldAcc + (newVal * scale);                                // UndCC_Violation
  }

  public static int multAccum1(int oldAcc, int newVal, int scale) {
    return safeAdd(oldAcc, safeMultiply(newVal, scale));
  }


  public static int multAccum2(int oldAcc, int newVal, int scale) {
    return Math.addExact(oldAcc, Math.multiplyExact(newVal, scale));
  }

  public static long intRangeCheck(long value) {
    if ((value < Integer.MIN_VALUE) || (value > Integer.MAX_VALUE)) {
      throw new ArithmeticException("Integer overflow");
    }
    return value;
  }

  public static int multAccum3(int oldAcc, int newVal, int scale) {
    final long res = intRangeCheck(
      ((long) oldAcc) + intRangeCheck((long) newVal * (long) scale)  // UndCC_FalsePositive - ranges of methods aren't checked
                                                                     // A positive number + a long from a method call could be an overflow.
    );
    return (int) res;
  }


  private static final BigInteger bigMaxInt =
    BigInteger.valueOf(Integer.MAX_VALUE);
  private static final BigInteger bigMinInt =
    BigInteger.valueOf(Integer.MIN_VALUE);

  public static BigInteger intRangeCheck(BigInteger val) {
    if (val.compareTo(bigMaxInt) == 1 ||
        val.compareTo(bigMinInt) == -1) {
      throw new ArithmeticException("Integer overflow");
    }
    return val;
  }

  public static int multAccum4(int oldAcc, int newVal, int scale) {
    BigInteger product =
      BigInteger.valueOf(newVal).multiply(BigInteger.valueOf(scale));
    BigInteger res =
      intRangeCheck(BigInteger.valueOf(oldAcc).add(product));
    return res.intValue();
  }

  class InventoryManager0 {
    private final AtomicInteger itemsInInventory = new AtomicInteger(100);
    public final void nextItem() {
      itemsInInventory.getAndIncrement();                            // UndCC_Violation
    }
  }

  class InventoryManager1 {
    private final AtomicInteger itemsInInventory =
        new AtomicInteger(100);

    public final void nextItem() {
      while (true) {
        int old = itemsInInventory.get();
        if (old == Integer.MAX_VALUE) {
          throw new ArithmeticException("Integer overflow");
        }
        int next = old + 1;                                          // UndCC_Valid
        if (itemsInInventory.compareAndSet(old, next)) {
          break;
        }
      }
    }
  }

  public static void main(String[] args) {

  }
}
