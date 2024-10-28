import java.io.*;
import java.lang.reflect.*;
import sun.misc.*;

public class UnsafeCode7 {
  public static void main(String[] args)
      throws SecurityException, NoSuchFieldException,
             IllegalArgumentException, IllegalAccessException {
    Field f = Unsafe.class.getDeclaredField("theUnsafe");
    f.setAccessible(true);
    Unsafe u = (Unsafe) f.get(null);
	IOException e = new IOException();
    u.throwException(e); // UndCC_Violation - cannot declare/catch Throwable
  }
}
