// Noncompliant Code Example (Class.newInstance())

public class UndeclaredException0 {
  public static void main(String[] args) {
    // No declared checked exceptions
    NewInstance0.undeclaredThrow( // UndCC_Valid - not tried
        new Exception("Any checked exception"));
  }
}

public class NewInstance0 {
  private static Throwable throwable;

  private NewInstance0() throws Throwable {
    throw throwable; // UndCC_Valid - declared & not tried
  }

  public static synchronized void undeclaredThrow(Throwable throwable) {
    // These exceptions should not be passed
    if (throwable instanceof IllegalAccessException ||
        throwable instanceof InstantiationException) {
      // Unchecked, no declaration required
      throw new IllegalArgumentException(); // UndCC_Valid - not tried
    }

    NewInstance0.throwable = throwable;
    try {
      // Next line throws the Throwable argument passed in above,
      // even though the throws clause of class.newInstance fails
      // to declare that this may happen
      NewInstance0.class.newInstance(); // UndCC_Violation - undeclared, tried, & uncaught Throwable through NewInstance0
    } catch (InstantiationException e) { /* Unreachable */
    } catch (IllegalAccessException e) { /* Unreachable */
    } finally { // Avoid memory leak
      NewInstance0.throwable = null;
    }
  }
}
