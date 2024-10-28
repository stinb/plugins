final class Test01 {
  final class Publisher {
    public Publisher self0;
    public volatile Publisher self1;

    Publisher(int number) {
      self0 = self1 = this; // UndCC_Violation
      // assignment to nonvolatile and nonfinal and public variable with 'this'
    }
  }

  final class ThreadStarter implements Runnable {
    public ThreadStarter() {
      Thread x, y;
      x = y = new java.lang.Thread(this); // UndCC_Violation
      // 'this' used to start thread in constructor
      x.start();
    }

    @Override public void run() {
      // ...
    }
  }
}
