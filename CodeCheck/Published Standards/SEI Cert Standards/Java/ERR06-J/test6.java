public class UndeclaredException6 {
  public static void main(String[] args) {
    // NewInstance6.undeclaredThrow0( // UndCC_Valid - not tried
    //     new Exception("Any checked exception"));
    // NewInstance6.undeclaredThrow1( // UndCC_Valid - not tried
    //     new Exception("Any checked exception"));

    try {
      NewInstance6.undeclaredThrow2( // UndCC_Valid - not tried
          new Exception("Any checked exception"));
    } catch (Throwable t) {

    }
  }
}

public class NewInstance6 {
  private static Throwable throwable;

  private NewInstance6() throws Throwable {
    throw throwable; // UndCC_Valid - declared & not tried
  }

  public static synchronized void undeclaredThrow0(Throwable throwable) {
    NewInstance6.throwable = throwable;
    try {
      NewInstance6.class.newInstance(); // UndCC_Valid - caught
    } catch (InstantiationException e) { /* Unreachable */
    } catch (IllegalAccessException e) { /* Unreachable */
    } catch (Throwable e) {
    } finally { // Avoid memory leak
      NewInstance6.throwable = null;
    }
  }

  public static synchronized void undeclaredThrow1(Throwable throwable) {
    NewInstance6.throwable = throwable;
    try {
      NewInstance6.class.newInstance(); // UndCC_Violation - undeclared, tried, & uncaught Throwable through NewInstance6
    } catch (InstantiationException e) { /* Unreachable */
    } catch (IllegalAccessException e) { /* Unreachable */
    } finally { // Avoid memory leak
      NewInstance6.throwable = null;
    }
  }

  public static synchronized void undeclaredThrow2(Throwable throwable) throws Throwable {
    NewInstance6.throwable = throwable;
    NewInstance6.class.newInstance(); // UndCC_Valid - declared
  }
}
