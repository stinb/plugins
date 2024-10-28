// Noncompliant Code Example (Class.newInstance() Workarounds)

import java.io.*;

public class UndeclaredException1 {
  public static void main(String[] args) {
    try {
      NewInstance1.undeclaredThrow( // UndCC_Valid - caught
          new IOException("Any checked exception"));
    } catch (Throwable e) {
      if (e instanceof IOException) {
        System.out.println("IOException occurred");
      } else if (e instanceof RuntimeException) {
        throw (RuntimeException) e; // UndCC_Valid - not tried
      } else {
        // Forward to handler
      }
    }
  }
}

public class NewInstance1 {
  private static Throwable throwable;

  private NewInstance1() throws Throwable {
    throw throwable; // UndCC_Valid - declared & not tried
  }

  public static synchronized void undeclaredThrow(Throwable throwable) {
    // These exceptions should not be passed
    if (throwable instanceof IllegalAccessException ||
        throwable instanceof InstantiationException) {
      // Unchecked, no declaration required
      throw new IllegalArgumentException(); // UndCC_Valid - not tried
    }

    NewInstance1.throwable = throwable;
    try {
      // Next line throws the Throwable argument passed in above,
      // even though the throws clause of class.newInstance fails
      // to declare that this may happen
      NewInstance1.class.newInstance(); // UndCC_Violation - undeclared, tried & uncaught Throwable through NewInstance0
    } catch (InstantiationException e) { /* Unreachable */
    } catch (IllegalAccessException e) { /* Unreachable */
    } finally { // Avoid memory leak
      NewInstance1.throwable = null;
    }
  }
}
