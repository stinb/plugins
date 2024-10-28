class Base {
  public synchronized void doSomething() {
    // ...
  }
}

class Derived extends Base {
  @Override public synchronized void doSomething() {  /* UndCC_Valid - method of the subclass is synced */
    // ...
  }
}

class Base3 {

  public synchronized void doSomething3() {
    // ...
  }
}

class Derived3 extends Base3 {
  private final Object lock = new Object();

  @Override public void doSomething3() {  /* UndCC_Valid - overridden method synced on a private final lock object */
    synchronized (lock) {
      // ...
    }
  }
}
