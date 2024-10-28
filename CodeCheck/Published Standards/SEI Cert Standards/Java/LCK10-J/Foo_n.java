// Double-checked locking idiom
final class Foo_n {
    private Helper helper = null;
    public Helper getHelper() { //UndCC_Violation
      if (helper == null) {
        synchronized (this) {
          if (helper == null) {
            helper = new Helper(1);
          }
        }
      }
      return helper;
    }
   
    // Other methods and members...
  }