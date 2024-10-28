public class UndeclaredException9 {
  public static void main(String[] args) {
    // NewInstance9.undeclaredThrow0( // UndCC_Valid - not tried
    //     new Exception("Any checked exception"));
    NewInstance9.undeclaredThrow1( // UndCC_Valid - not tried
         new Exception("Any checked exception"));

    try {
      NewInstance9.undeclaredThrow2( // UndCC_Valid - not tried
          new Exception("Any checked exception"));
    } catch (Throwable t) {

    }
  }
}

class NewInstance9 {
  private static Throwable throwable;

  private NewInstance9() throws Throwable {
    throw throwable; // UndCC_Valid - declared & not tried
  }

  static synchronized void undeclaredThrow0(Throwable throwable) {
    NewInstance9.throwable = throwable;
    try {
      NewInstance9.class.newInstance(); // UndCC_Valid - caught
    } catch (InstantiationException e) { /* Unreachable */
    } catch (IllegalAccessException e) { /* Unreachable */
    } catch (Throwable e) {
    } finally { // Avoid memory leak
      NewInstance9.throwable = null;
    }
  }

  static synchronized void undeclaredThrow1(Throwable throwable) {
    NewInstance9.throwable = throwable;
    try {
      Class myclass = NewInstance9.class;
      myclass.newInstance(); // UndCC_FalseNeg - undeclared, tried, & uncaught Throwable through NewInstance9
    } catch (InstantiationException e) { /* Unreachable */
    } catch (IllegalAccessException e) { /* Unreachable */
    } finally { // Avoid memory leak
      NewInstance9.throwable = null;
    }
  }

  static synchronized void undeclaredThrow2(Throwable throwable) throws Throwable {
    NewInstance9.throwable = throwable;
    NewInstance9.class.newInstance(); // UndCC_Valid - declared
  }
}
