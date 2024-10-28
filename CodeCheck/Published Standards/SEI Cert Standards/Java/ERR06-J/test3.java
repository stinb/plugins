// Noncompliant Code Example (sun.misc.Unsafe)

import java.io.IOException;
import java.lang.reflect.Field;
import sun.misc.Unsafe;

public class UnsafeCode3 {
  public static void main(String[] args)
      throws SecurityException, NoSuchFieldException,
             IllegalArgumentException, IllegalAccessException {
    Field f = Unsafe.class.getDeclaredField("theUnsafe");
    f.setAccessible(true);
    Unsafe u = (Unsafe) f.get(null);
    u.throwException(new IOException("No need to declare this checked exception")); // UndCC_Violation - cannot declare/catch IOException
  }
}
