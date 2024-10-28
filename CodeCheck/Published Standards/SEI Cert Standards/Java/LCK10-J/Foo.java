// Works with acquire/release semantics for volatile
// Broken under JDK 1.4 and earlier
final class Foo {
    private volatile Helper helper = null;
   
    public Helper getHelper() {
      if (helper == null) {
        synchronized (this) {
          if (helper == null) {
            helper = new Helper(1);
          }
        }
      }
      return helper;
    }
  }