class Test1 {

  private Object lock;
  private boolean condition0;
  private boolean condition1;

  private void threadSleep(long time) {
    Thread.sleep(time);
  }

  public void doSomething(long time) {
    while (condition0) {
      synchronized (lock) {
        if (condition1) {
          Thread.sleep(time); // UndCC_Violation
          threadSleep(time);  // UndCC_Violation
        }
      }
    }

    Thread.sleep(time); // UndCC_Valid
  }

}
