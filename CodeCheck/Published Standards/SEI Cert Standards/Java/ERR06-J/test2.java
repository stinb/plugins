// Compliant Solution (Constructor.newInstance())

import java.lang.reflect.*;

public class UndeclaredException2 {
  public static void main(String[] args) {
    // No declared checked exceptions
    NewInstance2.undeclaredThrow( // UndCC_Valid - not tried
        new Exception("Any checked exception"));
  }
}

public class NewInstance2 {
  private static Throwable throwable;

  private NewInstance2() throws Throwable {
    throw throwable; // UndCC_Valid - declared & not tried
  }

  public static synchronized void undeclaredThrow(Throwable throwable) {
    // These exceptions should not be passed
    if (throwable instanceof IllegalAccessException ||
        throwable instanceof InstantiationException) {
      // Unchecked, no declaration required
      throw new IllegalArgumentException(); // UndCC_Valid - not tried
    }

    NewInstance2.throwable = throwable;
    try {
      Constructor constructor =
          NewInstance2.class.getConstructor(new Class<?>[0]); // UndCC_Valid - error thrown by compiler
      constructor.newInstance(); // UndCC_Valid - see above
    } catch (InstantiationException e) { /* Unreachable */
    } catch (IllegalAccessException e) { /* Unreachable */
    } catch (InvocationTargetException e) {
      System.out.println("Exception thrown: "
          + e.getCause().toString());
    } finally { // Avoid memory leak
      NewInstance2.throwable = null;
    }
  }
}
