class Base2 {
  public synchronized void doSomething2() {
    // ...
  }
}

class Derived2 extends Base2 {
  @Override public void doSomething2() {  /* UndCC_Violation - base method ovverridden with an unsynced method in the derived class */
    // ...
  }
}

class Base4 {
  private final Object lock = new Object();

  public void doSomething4() {
    synchronized (lock) {
      // ...
    }
  }
}

class Derived4 extends Base4 {

  @Override public void doSomething4() {  /* UndCC_Violation - private final lock used only in base class */
    try {
      super.doSomething4();
    } finally {
      //...
    }
  }
}